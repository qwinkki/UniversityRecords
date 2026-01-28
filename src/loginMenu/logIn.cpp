#include "logIn.h"

void logIn(){
    std::string login, password;
    char choose, type;

    while(true) {
        clearScreen();

        std::cout << "If you want to exit program enter '0': ";
        std::cin >> choose; cinChar();
        if(choose == '0') return;
        
        clearScreen();

        std::cout << "Login: ";
        std::cin >> login;
        std::cout << "Password: ";
        std::cin >> password;

        type = getUserType(login);
        if(type == 'a'){
            if(getAdminFromDB(login, password))
                adminMenu(login);
            }
        else if(type == 'p'){
            professor professor = getProfessorFromDB(login, password);
            professorSelfMenu(professor);
        }
        else if(type == 's'){
            student student = getStudentFromDB(login, password);
            studentSelfMenu(student, login, password);
        }
        else if(type == 'n') std::cout << "Wrong login or password\n";
        else if(type == 'e') std::cout << "Error\n";
        
        wait();
    }
}