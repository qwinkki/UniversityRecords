#include "student.h"


void studentMenu(student st) {
	char choose;
	do {
		clearScreen();
		std::cout << "id: " << st.getId() << " | " << st.getName() << ' ' << st.getSurname();

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
			st.print();
			wait();
			break;
		case '2':
			clearScreen();
			st.printScore();
			wait();
			break;
		case '3':
			clearScreen();
			st.addSubject();
			wait();
			break;
		case '4':
			clearScreen();
			st.editMark();
			wait();
			break;
		case '5':
			clearScreen();
			st.deleteScore();
			wait();
			break;
		case '6':
			clearScreen();
			st.sortByName();
			wait();
			break;
		case '7':
			clearScreen();
			st.sortByMark();
			wait();
			break;
		case '8':
			clearScreen();
			st.startSession();
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