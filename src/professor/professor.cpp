#include "professor.h"

void professorSelfMenu(professor& professor){
    char choose; 
    std::vector<student> professorStudents = getStudentsByGroup(professor.getGroupCurator());

    do{
        clearScreen();
        professor.print();

        std::cout << "\nWhat do you want to do:"
            << "\n1. Show all information about professor"
            << "\n2. Edit students from " << professor.getGroupCurator()
            << "\n3. Start next year for students in group " << professor.getGroupCurator()
            << "\n\n0. Exit"
            << "\nEnter number: ";
            std::cin >> choose; cinChar();

        int studentId = 0;

        switch (choose)
        {
        case '1':
            clearScreen();
            professor.printAll();
            wait();
            break;
        case '2':
            clearScreen();
            for(auto& item : professorStudents){
                std::cout << studentId << ". ";
                item.print();
                studentId++;
            }
            std::cout << "\nEnter number of student to edit: ";
            std::cin >> studentId;

            if(studentId < 0 || studentId >= professorStudents.size()){
                std::cerr << COLORYELLOW << "Invalid student ID\n" << COLORDEFAULT;
                wait();
                break;
            }
            studentAdminMenu(professorStudents[studentId]);
            updateStudentInDB(professorStudents[studentId]);
            
            break;
        case '3':
            clearScreen();
            setStudentsNextYearByGroup(professor.getGroupCurator());
            std::cout << "All students in group '" << professor.getGroupCurator() << "' have been advanced to the next year.\n";

            wait();
            break;
        case '0':
            break;
        default:
			std::cout << COLORYELLOW << "Wront input" << COLORDEFAULT;
			wait();
			break;
        }

    }while(choose != '0');
}
void professorAdminMenu(professor& professor) {
    char choose;
    do {
        clearScreen();
        professor.print();

        std::cout << "\n\nWhat do you want to do:"
            << "\n1. Print everything about professor"
            << "\n2. Edit group curator"
            << "\n3. Edit subject"
            << "\n4. Start next year in group " << professor.getGroupCurator()
            << "\n\n0. Exit"
            << "\nEnter number: ";
        std::cin >> choose; cinChar();

        switch (choose)
        {
        case '1':
            clearScreen();
            professor.printAll();
            wait();
            break;
        case '2':
            clearScreen();
            {
                std::string newGroup;
                std::cout << "Enter new group curator: ";
                std::cin >> newGroup; cinChar();
                professor.setGroupCurator(newGroup);
                std::cout << COLORGREEN << "Group curator updated successfully\n" << COLORDEFAULT;
            }
            wait();
            break;
        case '3':
            clearScreen();
            {
                std::string newSubject;
                std::cout << "Enter new subject: ";
                std::cin >> newSubject; cinChar();
                professor.setSubject(newSubject);
                std::cout << COLORGREEN << "Subject updated successfully\n" << COLORDEFAULT;
            }
            wait();
            break;
        case '4':
            clearScreen();
            professor.nextYear();
            wait();
            break;
        case '0':
            break;
        default:
            std::cout << COLORYELLOW << "Wront input" << COLORDEFAULT;
            wait();
            break;
        }

        updateProfessorInDB(professor);
    } while (choose != '0');
}

// database 
void registerProfessorToDB(professor& professor, const std::string& login, const std::string& password)
{
    pqxx::work w(Database::getInstance());

    try {
        int groupId = getGroupId(w, professor.getGroupCurator());

        // check login
        pqxx::result check = w.exec_params(
            "SELECT 1 FROM users WHERE login = $1;",
            login
        );

        if (!check.empty())
            throw std::runtime_error("Login already exists");

        pqxx::result userReg = w.exec_params(
            "INSERT INTO users (login, password, role) "
            "VALUES ($1, $2, 'professor') RETURNING id;",
            login, password
        );

        int userId = userReg[0][0].as<int>();

        // subject
        pqxx::result subjectRes = w.exec_params(
            "SELECT id FROM subjects WHERE name = $1;",
            professor.getSubject()
        );

        int subjectId;
        if (!subjectRes.empty())
            subjectId = subjectRes[0][0].as<int>();
        else {
            pqxx::result newSubjectRes = w.exec_params(
                "INSERT INTO subjects (name) VALUES ($1) RETURNING id;",
                professor.getSubject()
            );
            subjectId = newSubjectRes[0][0].as<int>();
        }

        w.exec_params(
            "INSERT INTO professors (userId, name, surname, groupId, years, subjectId) "
            "VALUES ($1, $2, $3, $4, $5, $6);",
            userId,
            professor.getName(),
            professor.getSurname(),
            groupId,
            professor.getYearsInUniversity(),
            subjectId
        );

        w.commit();
        std::cout << "Professor registered successfully\n";
    }
    catch (const std::exception& e) {
        w.abort();
        std::cerr << COLORRED << e.what() << COLORDEFAULT << '\n';
    }
}

void updateProfessorInDB(const professor& professor){
    pqxx::work w(Database::getInstance());

	try{
        int groupId = getGroupId(w, professor.getGroupCurator());

        // subject
        pqxx::result subjectRes = w.exec_params("SELECT id FROM subjects WHERE name = $1;", professor.getSubject());
        int subjectId;
        if(!subjectRes.empty()){
            subjectId = subjectRes[0][0].as<int>();
        }
        else{
            pqxx::result newSubjectRes = w.exec_params("INSERT INTO subjects (name) VALUES ($1) RETURNING id;", professor.getSubject());
            subjectId = newSubjectRes[0][0].as<int>();
        }

		// update professor info
		w.exec_params("UPDATE professors SET name = $1, surname = $2, groupId = $3, years = $4, subjectId = $5 WHERE id = $6;",
			professor.getName(), professor.getSurname(), groupId, professor.getYearsInUniversity(), subjectId, professor.getId());
        
		w.commit();
		std::cout << COLORGREEN << "Professor " << professor.getName() << " succesfully updated in database\n" << COLORDEFAULT;
	}
	catch(const std::exception& e){
        w.abort();
	    std::cerr << COLORRED << e.what() << '\n' << COLORDEFAULT;
	}
}
professor getProfessorFromDB(const std::string& login, const std::string& password){
    pqxx::work w(Database::getInstance());

	try{
		pqxx::result userRes = w.exec_params("SELECT id, role FROM users WHERE login = $1 AND password = $2;", login, password);
        std::string role = userRes[0]["role"].as<std::string>();
        if(userRes.empty() || role != "professor") throw "Wrong login or password";
        int userId = userRes[0]["id"].as<int>();

        int subjectId = -1;
        // get subject id
        pqxx::result subjectRes = w.exec_params("SELECT subjectId FROM professors WHERE userId = $1;", userId);
        if(!subjectRes.empty()){
            subjectId = subjectRes[0][0].as<int>();
        }
        
        // get professor, group and subject
        pqxx::result professorRes = w.exec_params("SELECT p.id, p.name, p.surname, p.years, g.groupName, p.subjectId FROM professors p JOIN groups g ON p.groupId = g.id WHERE p.userId = $1 AND subjectId = $2;", 
            userId, subjectId);
        if(professorRes.empty()) throw "Professor profile not found";   

        professor prof(
            professorRes[0]["id"].as<int>(),
            professorRes[0]["name"].as<std::string>(),
            professorRes[0]["surname"].as<std::string>(),
            professorRes[0]["years"].as<int>(),
            professorRes[0]["groupName"].as<std::string>(),
            professorRes[0]["subjectId"].as<std::string>()
        );
        return prof;
	}
	catch(const std::exception& e){
        w.abort();
	    std::cerr << COLORRED << e.what() << '\n' << COLORDEFAULT;
		return professor(-1, "", "", -1, "", "");
	}
}