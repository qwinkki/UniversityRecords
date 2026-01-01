#include "professor.h"

void professorMenu(professor pr){
    char choose; 
    do{
        clearScreen();
        std::cout << "id: " << pr.getId() << " | " << pr.getName() << ' ' << pr.getSurname() 
            << "\tGroup: " << pr.getGroupCurator() << " Professor subject: " << pr.getSubject();

        std::cout << "\n\nWhat do you want to do:"
            << "\n1. Show all information about professor"
            << "\n2. "
            << "\n\n0. Exit"
            << "\nEnter number: ";
            std::cin >> choose; CINCHAR;

    }while(choose != '0');
}