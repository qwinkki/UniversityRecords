#include "members.h"

char getUserType(std::string& login){
	pqxx::work w(Database::getInstance());

	try{
		pqxx::result r = w.exec_params("SELECT role FROM users WHERE login = $1", login);

		if(r.empty()) return 'n';

		if(r[0][0].as<std::string>() == "admin") return 'a';
		else if(r[0][0].as<std::string>() == "professor") return 'p';
		else if (r[0][0].as<std::string>() == "student") return 's';
		else throw std::runtime_error("Invalid type of user");
	}
	catch(const std::exception& e){
		std::cerr << COLORRED << e.what() << '\n' << COLORDEFAULT;
		return 'e';
	}
}

int getGroupId(pqxx::work& w, const std::string& groupName)
{
    pqxx::result r = w.exec_params(
        "SELECT id FROM groups WHERE groupName = $1;",
        groupName
    );

    if (!r.empty())
        return r[0][0].as<int>();

    pqxx::result ins = w.exec_params(
        "INSERT INTO groups (groupName) VALUES ($1) RETURNING id;",
        groupName
    );

    return ins[0][0].as<int>();
}
