#include "main.h"

int main() {
	initializeDatabase();

    student stu1(1, "Alice", "Smith", 2, "IB-21");
    student stu2(2, "Bob", "Johnson", 3, "IB-21");
    student stu3(3, "Charlie", "Brown", 1, "IB-22");

    registerStudentToDB(stu1, "alice_login", "alice_pass");
    registerStudentToDB(stu2, "bob_login", "bob_pass");
    registerStudentToDB(stu3, "charlie_login", "charlie_pass");

    professor prof1(12, "John", "Doe", 10, "IB-21", "Mathematics");
    registerProfessorToDB(prof1, "john_login", "john_pass");

    professor a = getProfessorFromDB("john_login", "john_pass");
    a.printAll();
    wait();

    professorSelfMenu(a);

    return 0;
}