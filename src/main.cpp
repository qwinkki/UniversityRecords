#include "main.h"

int main() {
	initializeDatabase();

	student a(1, "Tom", "Soyer", 3, "IB");
	a.setScore({ {"Math", 4}, {"Physics", 5}, {"Literature", 3} });
    updateStudentInDB(a, "tomsoyer", "password123");
    a = getStudentFromDB("tomsoyer", "password123");
    a.printScore();
    a.setScore({ {"Math", 5}, {"Physics", 4}, {"Literature", 4} });
    updateStudentInDB(a, "tomsoyer", "password123");
    a = getStudentFromDB("tomsoyer", "password123");
    a.printScore();
    wait();


    return 0;
}