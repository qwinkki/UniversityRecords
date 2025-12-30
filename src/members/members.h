#pragma once

#include "common/common.h"

class member {

	unsigned int id;
	std::string name;
	std::string surname;
	unsigned int yearsInUniversity;

public:

	member(
		unsigned id,
		const std::string& name,
		const std::string& surname,
		unsigned int yearsInUniversity
	)
		: id(id),
		name(name),
		surname(surname),
		yearsInUniversity(yearsInUniversity)
	{ }

	unsigned int getId() const { return id; }
	std::string getName() const { return name; }
	std::string getSurname() const { return surname; }
	unsigned int getYearsInUniversity() const { return yearsInUniversity; }

	void setId(unsigned int _id) { id = _id; }
	void setYearsInUniversity(unsigned int year) { yearsInUniversity = year; }

	virtual void print() const {
		std::cout << "id: " << id << "  " << name << " " << surname << "  years of beeing in university: " << yearsInUniversity << "\n\n";
	}

	virtual ~member() = default;
};