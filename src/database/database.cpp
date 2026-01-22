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
		std::cout << "Initializing database.. " << COLORGREEN << "OK\n" << COLORDEFAULT;
		
		// login password
		w.exec("CREATE TABLE IF NOT EXISTS users (id SERIAL PRIMARY KEY, login TEXT NOT NULL UNIQUE, password TEXT NOT NULL, role TEXT NOT NULL CHECK (role IN ('admin', 'student', 'professor')));");
		std::cout << "users table.. " << COLORGREEN << "OK\n" << COLORDEFAULT;

		// admin
		w.exec("CREATE TABLE IF NOT EXISTS admins (id SERIAL PRIMARY KEY, userId INT NOT NULL UNIQUE, FOREIGN KEY (userId) REFERENCES users(id));");
		std::cout << "admins table.. " << COLORGREEN << "OK\n" << COLORDEFAULT;
		
		// groups, professors, students
		w.exec("CREATE TABLE IF NOT EXISTS groups (id SERIAL PRIMARY KEY, groupName TEXT NOT NULL UNIQUE);");
		w.exec("CREATE TABLE IF NOT EXISTS professors (id SERIAL PRIMARY KEY, userId INT NOT NULL, name TEXT NOT NULL, surname TEXT NOT NULL, groupId INT NOT NULL, FOREIGN KEY (userId) REFERENCES users(id), FOREIGN KEY (groupId) REFERENCES groups(id), years INT NOT NULL, subject TEXT NOT NULL);");
		w.exec("CREATE TABLE IF NOT EXISTS students (id SERIAL PRIMARY KEY, userId INT NOT NULL, name TEXT NOT NULL, surname TEXT NOT NULL, educationYear INT NOT NULL, group_id INT NOT NULL, FOREIGN KEY (userId) REFERENCES users(id), FOREIGN KEY (group_id) REFERENCES groups(id));");
		std::cout << "groups, professors, students tables.. " << COLORGREEN << "OK\n" << COLORDEFAULT;
		
		// marks for students
		w.exec("CREATE TABLE IF NOT EXISTS subjects (id SERIAL PRIMARY KEY, name TEXT NOT NULL UNIQUE);");
		w.exec("CREATE TABLE IF NOT EXISTS scores (id SERIAL PRIMARY KEY, studentId INT NOT NULL, subjectId INT NOT NULL, mark INT NOT NULL, FOREIGN KEY (studentId) REFERENCES students(id), FOREIGN KEY (subjectId) REFERENCES subjects(id), UNIQUE(studentId, subjectId));");
		std::cout << "subjects and scores tables.. " << COLORGREEN << "OK\n" << COLORDEFAULT;

		w.commit();

		wait();
	}
	catch (const std::exception& e) {
		std::cerr << COLORRED << e.what() << '\n' << COLORDEFAULT;
	}
}