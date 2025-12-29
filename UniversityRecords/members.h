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

class student : public member {

	std::vector<std::pair<std::string, unsigned short int>> score;

public:

	student(
		unsigned _id,
		const std::string& _name,
		const std::string& _surname,
		unsigned int _educationYear
	)
		: member(_id, _name, _surname, _educationYear)
	{}

	void addSubject(std::string subject, unsigned short int mark) {
		try {
			if (mark < 2 || mark > 5)
				throw std::out_of_range("Mark must bee between 2 and 5");

			score.push_back({ subject, mark });
		}
		catch (const std::exception& e) {
			std::cerr << COLORRED << e.what() << '\n' << COLORDEFAULT;
		}
	}

	void print() const override {
		std::cout << "id: " << getId() << '\t' << getName() << '\t' << getSurname() << "\t Graduation year: " << getYearsInUniversity() << '\n';
		if (!score.empty()) {
			std::cout << "Number of subjects: " << score.size();
			for (const auto& item : score)
				std::cout << "\n\tSubject: " << item.first << " Mark: " << item.second;
		}
		else
			std::cout << "No subjects yet\n";
		std::cout << '\n';
	}

};