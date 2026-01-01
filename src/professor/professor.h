#pragma once

#include "common/common.h"
#include "members/members.h"

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
    { }

    void setGroupCurator(std::string& newGroup) { groupCurator = newGroup; }
    void setSubject(std::string& newSubject) { subject = newSubject; }

    std::string getGroupCurator() const { return groupCurator; }
    std::string getSubject() const { return subject; }

    void nextYear(){
        setYearsInUniversity(getYearsInUniversity() + 1);
        std::cout << getName() << " teaching students for " << getYearsInUniversity() << " years\n";
    }

    void print() const override{
        std::cout << "\nProfessor"
            << " id: " << getId() << " | " << getName() << ' ' << getSurname() << " Group: " << getGroupCurator() << " | with subject: " << getSubject() 
            << "\n\tTeaching is university: " << getYearsInUniversity() << " years\n";
    }
};

void professorMenu(professor pr);