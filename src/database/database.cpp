#include "database.h"

// MAIN CONNECTION
pqxx::connection& Database::getInstance() {
	if (!conn) static Database instance;
	return *conn;
}
std::unique_ptr<pqxx::connection> Database::conn = nullptr;
Database::Database() {
	clearScreen();
	std::cout << "start connecting to Database\n";
	if(!conn)
		try {
			conn = std::make_unique<pqxx::connection>("postgresql://postgres:123@localhost:5432/UniversityMembers");
			if (conn->is_open())
				std::cout << "database opening.. " << COLORGREEN << "OK\n" << COLORDEFAULT;
			else {
				std::cout << COLORRED << "ERROR: Database in not opened\n" << COLORDEFAULT;
				exit(1);
			}
		}
		catch (const std::exception& e) {
			std::cerr << COLORRED << e.what() << '\n' << COLORDEFAULT;
			exit(1);
		}
}

void initializeDatabase() {
	try {
		pqxx::work w(Database::getInstance());
		std::cout << "initializing database.. " << COLORGREEN << "OK\n" << COLORDEFAULT;
	}
	catch (const std::exception& e) {
		std::cerr << COLORRED << e.what() << '\n' << COLORDEFAULT;
	}
}