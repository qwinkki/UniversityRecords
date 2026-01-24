#include "student.h"

void studentSelfMenu(student student){
	char choose;
	do{
		clearScreen();
		student.print();
		student.printScore();
		
		std::cout << "\n1. Refresh"
			<< "\n0. Exit"
			<< "\nEnter number: ";
		std::cin >> choose; CINCHAR;
		if(choose == '1'){
			// refresh marks from db
		}
	}while(choose != '0');
}

void studentMenu(student student) {
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
		std::cin >> choose; CINCHAR;

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

		// INSERT SUBJECTS

        w.commit();
        std::cout << "Student " << student.getName() << " succesfully registered\n";
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
		// GET SUBJECTS

		w.commit();

		return newStudent;
	}
	catch(const std::exception& e){
	    std::cerr << COLORRED << e.what() << '\n' << COLORDEFAULT;
		student errorStudent(-1, "", "", -1, "");
		return errorStudent;
	}
}