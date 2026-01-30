#include "admin.h"

void adminMenu(std::string& login){
    char choose;

    unsigned int usersChoose = 0;

    do{
        clearScreen();
        
        std::cout << "Admin panel as " << login 
            << "\nWhat do you want to do:"
            << "\n1. Show all users & edit them"
            << "\n2. Run tests"
            << "\n3. Start next year for all members"
            << "\n4. Check group, marks and subject extensions (clear if any issues found)"
            << "\n5. Register user (professor/student)"
            << "\n0. Exit"
            << "\nEnter number: ";
        std::cin >> choose; cinChar();

        switch(choose){
            case '1':
                clearScreen();
                getAllUsersFromDB();
                std::cout << "\nEnter role of user to edit (1. professors, 2. students): ";
                std::cin >> usersChoose; cinChar();
                if(usersChoose == 1){
                    std::cout << "Enter professor ID to edit: ";
                    std::cin >> usersChoose; cinChar();
                    
                    startEditingProfessor(usersChoose);
                } else if(usersChoose == 2){
                    std::cout << "Enter student ID to edit: ";
                    std::cin >> usersChoose; cinChar();
                    
                    startEditingStudent(usersChoose);
                } else{
                    std::cout << COLORYELLOW << "Invalid choice\n" << COLORDEFAULT;
                }
                wait();
                break;
            case '2':
                clearScreen();
                runTests();
                wait();
                break;
            case '3':
                clearScreen();
                {
                    pqxx::work w(Database::getInstance());
                    try{
                        w.exec("UPDATE students SET educationYear = educationYear + 1;");
                        w.exec("UPDATE professors SET years = years + 1;");
                        w.commit();
                        std::cout << COLORGREEN << "Started next year for all students and professors successfully\n" << COLORDEFAULT;
                    }
                    catch(const std::exception& e){
                        std::cerr << COLORRED << "Failed to start next year: " << e.what() << COLORDEFAULT << "\n\n";
                    }
                }
                wait();
                break;
            case '4':
                clearScreen();
                checkExtensionsAdmin();
                wait();
                break;
            case '5':
                clearScreen();
                createUser();
                wait();
                break;
            case '0':
                break;
            default:
                std::cout << COLORYELLOW << "Wront input" << COLORDEFAULT;
                wait();
                break;
        }
    } while(choose != '0');
}

// database
bool getAdminFromDB(std::string& login, std::string& password){
    pqxx::work w(Database::getInstance());

    try{
        pqxx::result r = w.exec_params("SELECT login, password FROM users WHERE role = 'admin' AND login = $1 AND password = $2 LIMIT 1;", 
            login, password);
        if(r.empty()){
            std::cout << COLORYELLOW << "Admin not found with given login and password\n" << COLORDEFAULT;
            return false;
        }
        w.commit();
        return true;
    }
    catch(const std::exception& e){
        std::cerr << COLORRED << e.what() << '\n' << COLORDEFAULT;
        return false;
    }
}
void getAllUsersFromDB(){
    pqxx::work w(Database::getInstance());

    try{
        pqxx::result r = w.exec("SELECT id, name, surname FROM professors ORDER BY id;");
        if(r.empty()){
            std::cout << COLORYELLOW << "No professors found in database\n" << COLORDEFAULT;
        }
        std::cout << "All professors in database:\n";
        for(const auto& row : r){
            std::cout << "ID: " << row["id"].as<int>()
                << " | Name: " << row["name"].as<std::string>()
                << " | Surname: " << row["surname"].as<std::string>() << '\n';
        }

        r = w.exec("SELECT id, name, surname FROM students ORDER BY id;");
        if(r.empty()){
            std::cout << COLORYELLOW << "No students found in database\n" << COLORDEFAULT;
        }
        std::cout << "All students in database:\n";
        for(const auto& row : r){
            std::cout << "ID: " << row["id"].as<int>()
                << " | Name: " << row["name"].as<std::string>()
                << " | Surname: " << row["surname"].as<std::string>() << '\n';
        }
        w.commit();
    }
    catch(const std::exception& e){
        std::cerr << COLORRED << e.what() << '\n' << COLORDEFAULT;
    }
}


// edit users
void startEditingProfessor(int professorId){
    pqxx::work w(Database::getInstance());

    try{
        pqxx::result r = w.exec_params("SELECT p.id, p.name, p.surname, p.years, p.subject, g.groupName "
            "FROM professors p "
            "JOIN groups g ON p.groupId = g.id "
            "WHERE p.id = $1;", 
            professorId);
        if(r.empty()){
            std::cout << COLORYELLOW << "Professor with ID " << professorId << " not found\n" << COLORDEFAULT;
            return;
        }
        professor prof(
            r[0]["id"].as<int>(),
            r[0]["name"].as<std::string>(),
            r[0]["surname"].as<std::string>(),
            r[0]["years"].as<int>(),
            r[0]["subject"].as<std::string>(),
            r[0]["groupName"].as<std::string>()
        );

        professorAdminMenu(prof);
        w.commit();
    }
    catch(const std::exception& e){
        std::cerr << COLORRED << e.what() << '\n' << COLORDEFAULT;
    }
}
void startEditingStudent(int studentId){
    pqxx::work w(Database::getInstance());

    try{
        pqxx::result r = w.exec_params("SELECT s.id, s.name, s.surname, s.educationYear, g.groupName "
            "FROM students s "
            "JOIN groups g ON s.groupId = g.id "
            "WHERE s.id = $1;", 
            studentId);
        if(r.empty()){
            std::cout << COLORYELLOW << "Student with ID " << studentId << " not found\n" << COLORDEFAULT;
            return;
        }
        student stud(
            r[0]["id"].as<int>(),
            r[0]["name"].as<std::string>(),
            r[0]["surname"].as<std::string>(),
            r[0]["educationYear"].as<int>(),
            r[0]["groupName"].as<std::string>()
        );

        std::vector<std::pair<std::string, unsigned short int>> scores;
        pqxx::result scoresRes = w.exec_params(
            "SELECT sub.name, sc.mark FROM scores sc JOIN subjects sub ON sc.subjectId = sub.id WHERE sc.studentId = $1;",
            studentId);
        if(!scoresRes.empty()){
            for(const auto& row : scoresRes){
                scores.emplace_back(row["name"].as<std::string>(), row["mark"].as<unsigned short int>());
            }
            stud.setScore(scores);
        }

        studentAdminMenu(stud);
        w.commit();
    }
    catch(const std::exception& e){
        std::cerr << COLORRED << e.what() << '\n' << COLORDEFAULT;
    }
}

// tests
void runTests(){
    std::cout << "Starting tests...\n\n";
    
    // add a group
    std::cout << "Test 1: Adding a group...\n";
    try {
        pqxx::work w(Database::getInstance());
        pqxx::result groupRes = w.exec_params("INSERT INTO groups (groupName) VALUES ($1) RETURNING id;", "testGroup");
        int groupId = groupRes[0][0].as<int>();
        w.commit();
        std::cout << COLORGREEN << "Group added successfully with ID: " << groupId << COLORDEFAULT << "\n\n";
    } catch (const std::exception& e) {
        std::cerr << COLORRED << "Failed to add group: " << e.what() << COLORDEFAULT << "\n\n";
    }
    
    // add a professor
    std::cout << "Test 2: Adding a professor...\n";
    try {
        professor testProf(0, "testName", "testSurname", 5, "testGroup", "Mathematics");
        registerProfessorToDB(testProf, "prof_test", "prof123");
        std::cout << COLORGREEN << "Professor added successfully" << COLORDEFAULT << "\n\n";
    } catch (const std::exception& e) {
        std::cerr << COLORRED << "Failed to add professor: " << e.what() << COLORDEFAULT << "\n\n";
    }

    // add a student
    std::cout << "Test 3: Adding a student...\n";
    try {
        student testStud(0, "testStudent", "testSurname", 2, "testGroup");
        // Add some test marks
        testStud.setScore({{"test1", 4}, {"test2", 5}, {"test3", 3}});
        registerStudentToDB(testStud, "stud_test", "stud123");
        std::cout << COLORGREEN << "Student added successfully" << COLORDEFAULT << "\n\n";
    } catch (const std::exception& e) {
        std::cerr << COLORRED << "Failed to add student: " << e.what() << COLORDEFAULT << "\n\n";
    }

    // add marks to existing student
    std::cout << "Test 4: Adding additional marks to student...\n";
    try {
        pqxx::work w(Database::getInstance());
        // get student ID from database
        pqxx::result studentRes = w.exec_params(
            "SELECT s.id FROM students s "
            "JOIN users u ON s.userId = u.id "
            "WHERE u.login = $1;", 
            "stud_test"
        );
        
        if (!studentRes.empty()) {
            int studentId = studentRes[0][0].as<int>();
            
            // check if subject exists
            pqxx::result subjectRes = w.exec_params("SELECT id FROM subjects WHERE name = $1;", "testSubject");
            int subjectId;
            
            if (subjectRes.empty()) {
                pqxx::result newSubRes = w.exec_params("INSERT INTO subjects (name) VALUES ($1) RETURNING id;", "testSubject");
                subjectId = newSubRes[0][0].as<int>();
            } else {
                subjectId = subjectRes[0][0].as<int>();
            }
            
            // insert the mark
            w.exec_params("INSERT INTO scores (studentId, subjectId, mark) VALUES ($1, $2, $3);",
                studentId, subjectId, 5);
            
            w.commit();
            std::cout << COLORGREEN << "Marks added successfully" << COLORDEFAULT << "\n\n";
        } else {
            std::cout << COLORYELLOW << "Student not found" << COLORDEFAULT << "\n\n";
        }
    } catch (const std::exception& e) {
        std::cerr << COLORRED << "Failed to add marks: " << e.what() << COLORDEFAULT << "\n\n";
    }
    
    wait();

    // delete all test data
    std::cout << "Clearing up test data...\n";
    try {
        pqxx::work w(Database::getInstance());
        
        w.exec_params("DELETE FROM scores WHERE studentId IN (SELECT s.id FROM students s JOIN users u ON s.userId = u.id WHERE u.login IN ('stud_test'));");
        
        w.exec_params("DELETE FROM students WHERE userId IN (SELECT id FROM users WHERE login = 'stud_test');");
        
        w.exec_params("DELETE FROM professors WHERE userId IN (SELECT id FROM users WHERE login = 'prof_test');");
        
        w.exec_params("DELETE FROM subjects WHERE name IN ('test1', 'test2', 'test3', 'testSubject');");
        
        w.exec_params("DELETE FROM users WHERE login IN ('stud_test', 'prof_test');");
        
        w.exec_params("DELETE FROM groups WHERE groupName = 'testGroup';");
        
        w.commit();
        std::cout << COLORGREEN << "All test data deleted successfully" << COLORDEFAULT << "\n";
    } catch (const std::exception& e) {
        std::cerr << COLORRED << "Failed to cleanup test data: " << e.what() << COLORDEFAULT << "\n";
    }
    
    std::cout << "Tests completed.\n";
}

void checkExtensionsAdmin(){
    std::cout << "Starting extension checks...\n\n";
    pqxx::work w(Database::getInstance());

    try{
        std::cout << "Checking scores without valid students or subjects...\n";

        pqxx::result r = w.exec(
            "DELETE FROM scores "
            "WHERE studentId NOT IN (SELECT id FROM students) "
            "OR subjectId NOT IN (SELECT id FROM subjects);"
        );
        std::cout << COLORGREEN << "Scores extension check completed successfully\n" << COLORDEFAULT;

        std::cout << "Checking subjects without assigned professors and students...\n";
        r = w.exec(
            "DELETE FROM subjects "
            "WHERE id NOT IN (SELECT DISTINCT subjectId FROM scores);"
            "DELETE FROM subjects "
            "WHERE id NOT IN (SELECT DISTINCT subjectId FROM professors);"
        );
        std::cout << COLORGREEN << "Subjects extension check completed successfully\n" << COLORDEFAULT;

        std::cout << "Checking groups without assigned professors and students...\n";
        r = w.exec(
            "DELETE FROM groups "
            "WHERE id NOT IN (SELECT DISTINCT groupId FROM professors);"
            "DELETE FROM groups "
            "WHERE id NOT IN (SELECT DISTINCT groupId FROM students);"
        );
        std::cout << COLORGREEN << "Groups extension check completed successfully\n" << COLORDEFAULT;

        std::cout << "\nAll extension checks completed.\n";
        w.commit();
    }
    catch(const std::exception& e){
        std::cerr << COLORRED << "Error checking extensions: " << e.what() << COLORDEFAULT << "\n";
    }
}

void createUser(){
    std::cout << "Creating a new user...\n";
    char roleChoice;
    std::string login, password;

    std::cout << "Enter role of user to create (1. professor, 2. student): ";
    std::cin >> roleChoice; cinChar();

    std::cout << "Enter login: ";
    std::getline(std::cin, login);
    std::cout << "Enter password: ";
    std::getline(std::cin, password);

    if(roleChoice == '1'){
        
    } else if(roleChoice == '2'){
        
    } 
}