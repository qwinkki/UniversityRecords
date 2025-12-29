#include "main.h"

int main() {
	member a(1, "bob", "rodgers", 2);
	a.print();

	student b(2, "peter", "regret", 3);
	b.addSubject("math", 4);
	b.addSubject("asdasd", 1);
	b.addSubject("sdasd", 7);
	b.addSubject("sadasd", 3);
	b.print();
}