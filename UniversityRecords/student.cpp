#include "student.h"


void studentMenu(student st) {
	char choose;
	do {
		system("cls");
		std::cout << "id: " << st.getId() << " | " << st.getName() << ' ' << st.getSurname();

		std::cout << "\n\nWhat do you want to do:"
			<< "\n1. Print everything about student"
			<< "\n2. Print only scores"
			<< "\n3. Add subject"
			<< "\n4. Change mark"
			<< "\n5. Delete subject"
			<< "\n0. Exit"
			<< "\nEnter number: ";
		std::cin >> choose; CINCHAR;

		switch (choose)
		{
		case '1':
			system("cls");
			st.print();
			system("pause");
			break;
		case '2':
			system("cls");
			st.printScore();
			system("pause");
			break;
		case '3':
			system("cls");
			st.addSubject();
			system("pause");
			break;
		case '4':
			system("cls");
			st.editMark();
			system("pause");
			break;
		case '5':
			system("cls");
			st.deleteScore();
			system("pause");
			break;
		case '0':
			break;
		default:
			std::cout << COLORYELLOW << "Wront input" << COLORDEFAULT;
			system("pause");
			break;
		}
	} while (choose != '0');
}