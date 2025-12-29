#pragma once

#include "common.h"

class member {

	unsigned int id;
	std::string name;
	std::string surname;
	unsigned int yearsInUniversity;

public:

	member(
		unsigned _id,
		const std::string& _name,
		const std::string& _surname,
		unsigned int _yearsInUniversity
	)
		: id(_id),
		name(_name),
		surname(_surname),
		yearsInUniversity(_yearsInUniversity)
	{ }

	unsigned int getId() const { return id; }
	std::string getName() const { return name; }
	std::string getSurname() const { return surname; }
	unsigned int getYearsInUniversity() const { return yearsInUniversity; }

	void setId(unsigned int _id) { id = _id; }

	virtual void print() const {
		std::cout << "id: " << id << "  " << name << " " << surname << "  years of beeing in university: " << yearsInUniversity << "\n\n";
	}

	virtual ~member() = default;
};