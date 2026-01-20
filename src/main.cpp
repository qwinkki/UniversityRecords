#include "main.h"

int main() {

	student a(1, "tom", "soyer", 2, "IB");

	initializeDatabase();
	
	studentMenu(a);
}