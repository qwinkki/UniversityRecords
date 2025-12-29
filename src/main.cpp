#include "main.h"

int main() {

	initializeDatabase();
	wait();	

	student a(1, "tom", "soyer", 2);

	studentMenu(a);
}