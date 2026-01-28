#include "student.h"

void studentSelfMenu(student& student, std::string& login, std::string& password){
	char choose;
	do{
		clearScreen();
		student.print();
		student.printScore();
		
		std::cout << "\n1. Refresh"
			<< "\n2. Show information about your professors"
			<< "\n0. Exit"
			<< "\nEnter number: ";
		std::cin >> choose; cinChar();
		if(choose == '1'){
			getStudentFromDB(login, password);
		} else if(choose == '2'){
			// update professor
		}
	}while(choose != '0');
}

void studentAdminMenu(student& student) {
	char choose;
	do {
		clearScreen();
		student.print();

		std::cout << "\n\nWhat do you want to do:"
			<< "\n1. Print everything about student"
			<< "\n2. Print only scores"
			<< "\n3. Add subject"
			<< "\n4. Change mark"
			<< "\n5. Delete subject"
			<< "\n6. Sort by name of subject"
			<< "\n7. Sort by mark"
			<< "\n8. Start session"
			<< "\n\n0. Exit"
			<< "\nEnter number: ";
		std::cin >> choose; cinChar();

		switch (choose)
		{
		case '1':
			clearScreen();
			student.printAll();
			wait();
			break;
		case '2':
			clearScreen();
			student.printScore();
			wait();
			break;
		case '3':
			clearScreen();
			student.addSubject();
			wait();
			break;
		case '4':
			clearScreen();
			student.editMark();
			wait();
			break;
		case '5':
			clearScreen();
			student.deleteScore();
			wait();
			break;
		case '6':
			clearScreen();
			student.sortByName();
			wait();
			break;
		case '7':
			clearScreen();
			student.sortByMark();
			wait();
			break;
		case '8':
			clearScreen();
			student.startSession();
			wait();
			break;
		case '0':
			break;
		default:
			std::cout << COLORYELLOW << "Wront input" << COLORDEFAULT;
			wait();
			break;
		}
	} while (choose != '0');
}

void registerStudentToDB(student& student, const std::string& login, const std::string& password){
    int groupId = getGroupId(student.getGroup());

    if (groupId == -1) {
        std::cerr << COLORRED << "Could not find or create a group ID.\n" << COLORDEFAULT;
            return;
    }

	pqxx::work w(Database::getInstance());

    try{
        // check login
        pqxx::result check = w.exec_params("SELECT 1 FROM users WHERE login = $1;", login);

        if(!check.empty()){
            std::cerr << COLORYELLOW << "Login already exists\n" << COLORDEFAULT;
            return;
        }

        // creating user
        pqxx::result userReg = w.exec_params("INSERT INTO users (login, password, role) VALUES ($1, $2, 'student') RETURNING id;",
            login, password);

        unsigned int userId = userReg[0][0].as<int>();

        // creating student
        w.exec_params("INSERT INTO students (userId, name, surname, educationYear, groupId) VALUES ($1, $2, $3, $4, $5);",
            userId, student.getName(), student.getSurname(), student.getYearsInUniversity(), groupId);

		// subjects
		if(student.getScore().size() > 0){
			for(auto& item : student.getScore()){
				pqxx::result subjectRes = w.exec_params("SELECT id FROM subjects WHERE name = $1;", item.first);

				int hasSubjectId;
				if(!subjectRes.empty()){
					hasSubjectId = subjectRes[0][0].as<int>();
				}
				else{
					pqxx::result newSubjectRes = w.exec_params("INSERT INTO subjects (name) VALUES ($1) RETURNING id;", item.first);
					hasSubjectId = newSubjectRes[0][0].as<int>();
				}

				w.exec_params("INSERT INTO scores (studentId, subjectId, mark) VALUES ((SELECT id FROM students WHERE userId = $1), $2, $3);",
					userId, hasSubjectId, item.second);
			}
		}

        w.commit();
        std::cout << "Student " << student.getName() << " succesfully registered\n";
    }
    catch(const std::exception& e){
	    std::cerr << COLORRED << e.what() << '\n' << COLORDEFAULT;
    }
}
void updateStudentInDB(const student& student, const std::string& login, const std::string& password){
	pqxx::work w(Database::getInstance());

	try{
		// verify user
		pqxx::result userRes = w.exec_params("SELECT id, role FROM users WHERE login = $1 AND password = $2;", login, password);
		std::string role = userRes[0]["role"].as<std::string>();
		if(userRes.empty() || role != "student") throw "Wrong login or password";

		// update student info
		w.exec_params("UPDATE students SET name = $1, surname = $2, educationYear = $3 WHERE id = $4;",
			student.getName(), student.getSurname(), student.getYearsInUniversity(), student.getId());

		// update scores
		// first delete existing scores
		w.exec_params("DELETE FROM scores WHERE studentId = $1;", student.getId());

		// then insert new scores
		for(const auto& item : student.getScore()){
			pqxx::result subjectRes = w.exec_params("SELECT id FROM subjects WHERE name = $1;", item.first);

			int subjectId;
			if(!subjectRes.empty()){
				subjectId = subjectRes[0][0].as<int>();
			}
			else{
				pqxx::result newSubjectRes = w.exec_params("INSERT INTO subjects (name) VALUES ($1) RETURNING id;", item.first);
				subjectId = newSubjectRes[0][0].as<int>();
			}

			w.exec_params("INSERT INTO scores (studentId, subjectId, mark) VALUES ($1, $2, $3);",
				student.getId(), subjectId, item.second);
		}

		w.commit();
		std::cout << "Student " << student.getName() << " succesfully updated in database\n";
	}
	catch(const std::exception& e){
	    std::cerr << COLORRED << e.what() << '\n' << COLORDEFAULT;
	}
}
student getStudentFromDB(const std::string& login, const std::string& password){
	pqxx::work w(Database::getInstance());

	try{
		pqxx::result userRes = w.exec_params("SELECT id, role FROM users WHERE login = $1 AND password = $2;", login, password);
		std::string role = userRes[0]["role"].as<std::string>();
		if(userRes.empty() || role != "student") throw "Wrong login or password";

		int userId = userRes[0]["id"].as<int>();
		// get student and group
		pqxx::result studentRes = w.exec_params("SELECT s.id, s.name, s.surname, s.educationYear, g.groupName FROM students s JOIN groups g ON s.groupId = g.id WHERE s.userId = $1;", userId);
		if(studentRes.empty()) throw "Student profile not found";
		
		student newStudent(studentRes[0]["id"].as<int>(), studentRes[0]["name"].as<std::string>(), studentRes[0]["surname"].as<std::string>(), studentRes[0]["educationYear"].as<int>(), studentRes[0]["groupName"].as<std::string>());
		
		// get scores
		pqxx::result scoresRes = w.exec_params(
			"SELECT sub.name, sc.mark FROM scores sc JOIN subjects sub ON sc.subjectId = sub.id WHERE sc.studentId = $1;",
			studentRes[0]["id"].as<int>());
		if(!scoresRes.empty()){
			std::vector<std::pair<std::string, unsigned short int>> scores;
			for(const auto& row : scoresRes){
				scores.emplace_back(row["name"].as<std::string>(), row["mark"].as<unsigned short int>());
			}
			newStudent.setScore(scores);
		}

		w.commit();

		return newStudent;
	}
	catch(const std::exception& e){
	    std::cerr << COLORRED << e.what() << '\n' << COLORDEFAULT;
		student errorStudent(-1, "", "", -1, "");
		return errorStudent;
	}
}
std::vector<student> getStudentsByGroup(const std::string& group){
	pqxx::work w(Database::getInstance());
	std::vector<student> studentsByGroup;

	try{
		pqxx::result r = w.exec_params("SELECT s.id, s.name, s.surname, s.educationYear, g.groupName "
			"FROM students s "
			"JOIN groups g ON s.groupId = g.id "
			"WHERE g.groupName = $1;", 
			group);
		
		if(r.empty()){
			std::cout << COLORYELLOW << "No students found in group '" << group << "'\n" << COLORDEFAULT;
			return studentsByGroup;
		}
		for(const auto& row : r){
			studentsByGroup.emplace_back(
				row["id"].as<int>(),
				row["name"].as<std::string>(),
				row["surname"].as<std::string>(),
				row["educationYear"].as<int>(),
				row["groupName"].as<std::string>()
			);
		}

		return studentsByGroup;
	}
	catch(const std::exception& e){
	    std::cerr << COLORRED << e.what() << '\n' << COLORDEFAULT;
		return studentsByGroup;
	}
}
void setStudentsNextYearByGroup(const std::string& group){
	pqxx::work w(Database::getInstance());

	try{
		w.exec_params("UPDATE students SET educationYear = educationYear + 1 WHERE groupId = (SELECT id FROM groups WHERE groupName = $1);",
			group);

		w.commit();
		std::cout << "All students in group '" << group << "' have been advanced to the next year.\n";
	}
	catch(const std::exception& e){
	    std::cerr << COLORRED << e.what() << '\n' << COLORDEFAULT;
	}
}