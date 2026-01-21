#include "professor.h"

void professorMenu(professor professor, std::vector<student> students){
    char choose; 
    do{
        clearScreen();
        std::cout << "id: " << professor.getId() << " | " << professor.getName() << ' ' << professor.getSurname() 
            << "\tGroup: " << professor.getGroupCurator() << " Professor subject: " << professor.getSubject();

        std::cout << "\n\nWhat do you want to do:"
            << "\n1. Show all information about professor"
            << "\n2. Edit students from " << professor.getGroupCurator()
            << "\n3. "
            << "\n\n0. Exit"
            << "\nEnter number: ";
            std::cin >> choose; CINCHAR;

        switch (choose)
        {
        case '1':
            clearScreen();
            professor.printAll();
            wait();
            break;
        
        default:
			std::cout << COLORYELLOW << "Wront input" << COLORDEFAULT;
			wait();
			break;
        }

    }while(choose != '0');
}