#pragma once

#include "common/common.h"
#include "members/members.h"
#include "student/student.h"

class professor : public member
{
    std::string groupCurator;
    std::string subject;

    public:
    professor(
        unsigned id,
        const std::string& name,
        const std::string& surname,
        unsigned int yearsInUniversity,
        const std::string& groupCurator,
        const std::string& subject
    )
        : member(id, name, surname, yearsInUniversity),
          groupCurator(groupCurator),
          subject(subject)
    {
    }
    professor(const professor& other)
        : member(other),
          groupCurator(other.groupCurator),
          subject(other.subject)
    {
    }

    void setGroupCurator(const std::string& newGroup) { groupCurator = newGroup; }
    void setSubject(const std::string& newSubject) { subject = newSubject; }

    std::string getGroupCurator() const { return groupCurator; }
    std::string getSubject() const { return subject; }

    void nextYear(){
        setYearsInUniversity(getYearsInUniversity() + 1);
        std::cout << getName() << " teaching students for " << getYearsInUniversity() << " years\n";
    }

    // print
    void print() const override{
        std::cout << "id: " << getId() << "\tProfessor: " << getName() << ' ' << getSurname() << "\tGroup: " << getGroupCurator() << '\n';
    }
    void printAll() const override{
        std::cout << "\nProfessor"
            << " id: " << getId() << " | " << getName() << ' ' << getSurname() << " Group: " << getGroupCurator() << " | with subject: " << getSubject() 
            << "\n\tTeaching is university: " << getYearsInUniversity() << " years\n";
    }
};

// menu
void professorSelfMenu(professor professor);
void professorAdminMenu(professor& professor);

// database
void registerProfessorToDB(professor& professor, const std::string& login, const std::string& password);
void updateProfessorInDB(const professor& professor, const std::string& login, const std::string& password);
professor getProfessorFromDB(const std::string& login, const std::string& password);