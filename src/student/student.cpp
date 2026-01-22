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