#include "main.h"

int main() {
	initializeDatabase();

	student a(1, "Tom", "Soyer", 3, "IB");
	student b(2, "b", "b", 3, "KB");
	student c(3, "c", "c", 3, "abc");
    clearScreen();
	
    registerStudentToDB(a, "test", "test");
    registerStudentToDB(b, "test2", "test2");
    registerStudentToDB(c, "test3", "123");

    clearScreen();
    a.print();
    a = getStudentFromDB("test2", "test2");
    a.print();


    return 0;
}