#include "members.h"

int getGroupId(const std::string groupName){
	pqxx::work w(Database::getInstance());
	
try{
		pqxx::result r = w.exec_params("SELECT id FROM groups WHERE groupName = $1", groupName);
		int id;
		if(!r.empty())
			id = r[0][0].as<int>();
		else{
			// if dont exists, create
			pqxx::result ins = w.exec_params("INSERT INTO groups (groupName) VALUES ($1) RETURNING id;",
				groupName);
			id = ins[0][0].as<int>();
		}
		w.commit();
		return id;
	}
	catch(const std::exception& e){
		std::cerr << COLORRED << e.what() << '\n' << COLORDEFAULT;
		return -1;
	}
}

