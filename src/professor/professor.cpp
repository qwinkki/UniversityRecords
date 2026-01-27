#include "professor.h"

void professorSelfMenu(professor professor){
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
    } while (choose != '0');
}

// database 
void registerProfessorToDB(professor& professor, const std::string& login, const std::string& password){
    int groupId = getGroupId(professor.getGroupCurator());
    
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
        pqxx::result userReg = w.exec_params("INSERT INTO users (login, password, role) VALUES ($1, $2, 'professor') RETURNING id;",
            login, password);

        unsigned int userId = userReg[0][0].as<int>();

        // creating professor
        w.exec_params("INSERT INTO professors (userId, name, surname, groupId, years, subject) VALUES ($1, $2, $3, $4, $5, $6);",
            userId, professor.getName(), professor.getSurname(), groupId, professor.getYearsInUniversity(), professor.getSubject());

        w.commit();
        std::cout << "Professor " << professor.getName() << " succesfully registered\n";
    }
    catch(const std::exception& e){
	    std::cerr << COLORRED << e.what() << '\n' << COLORDEFAULT;
    }
}
void updateProfessorInDB(const professor& professor, const std::string& login, const std::string& password){
    int groupId = getGroupId(professor.getGroupCurator());
    
    if (groupId == -1) {
        std::cerr << COLORRED << "Could not find or create a group ID.\n" << COLORDEFAULT;
        return;
    }

    pqxx::work w(Database::getInstance());

	try{
		// verify user
		pqxx::result userRes = w.exec_params("SELECT id, role FROM users WHERE login = $1 AND password = $2;", login, password);
		std::string role = userRes[0]["role"].as<std::string>();
		if(userRes.empty() || role != "professor") throw "Wrong login or password";

		// update professor info
		w.exec_params("UPDATE professors SET name = $1, surname = $2, groupId = $3, years = $4, subject = $5 WHERE id = $6;",
			professor.getName(), professor.getSurname(), groupId, professor.getYearsInUniversity(), professor.getSubject(), professor.getId());

		w.commit();
		std::cout << "Professor " << professor.getName() << " succesfully updated in database\n";
	}
	catch(const std::exception& e){
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
        
        // get professor and group
        pqxx::result professorRes = w.exec_params("SELECT p.id, p.name, p.surname, p.years, g.groupName, p.subject FROM professors p JOIN groups g ON p.groupId = g.id WHERE p.userId = $1;", userId);
        if(professorRes.empty()) throw "Professor profile not found";   
        professor prof(
            professorRes[0]["id"].as<int>(),
            professorRes[0]["name"].as<std::string>(),
            professorRes[0]["surname"].as<std::string>(),
            professorRes[0]["years"].as<int>(),
            professorRes[0]["groupName"].as<std::string>(),
            professorRes[0]["subject"].as<std::string>()
        );
        return prof;
	}
	catch(const std::exception& e){
	    std::cerr << COLORRED << e.what() << '\n' << COLORDEFAULT;
		return professor(-1, "", "", -1, "", "");
	}
}