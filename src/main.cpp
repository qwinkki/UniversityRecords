#include "main.h"

int main() {

	student a(1, "tom", "soyer", 2, "IB");
	student b(2, "bob", "phil", 3, "KB");
	student c(3, "third", "okak", 4, "IB");

	std::vector<student> students;
	students.push_back(a);
	students.push_back(b);
	students.push_back(c);

	clearScreen();
	std::cout << "1. Show all students"
		<< "\n2. Choose by id"
		<< "\n3. Delete student\n";
	char choose;
	std::cin >> choose; CINCHAR;

	if(choose == '1'){
		clearScreen();
		for(auto& item : students)
			item.print();
		wait();
	}else if(choose == '2'){
		clearScreen();
		unsigned chooseId;
		std::cin >> chooseId;
		studentMenu(students[chooseId - 1]);
	}
}