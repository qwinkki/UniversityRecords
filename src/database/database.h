#pragma once
#include "common/common.h"

#include <pqxx/pqxx>

class Database {
	Database();
	static std::unique_ptr<pqxx::connection> conn;
public:
	static pqxx::connection& getInstance();
};

void initializeDatabase();