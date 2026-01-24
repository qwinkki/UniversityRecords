#include <algorithm>

#include "common/common.h"
#include "members/members.h"
#include "database/database.h"

class student : public member {

    std::vector<std::pair<std::string, unsigned short int>> score;
    std::string group;
    bool isGraduating = true;

    bool showScores() const {
        if (!score.empty()) {
            std::cout << "Current subjects (" << score.size() << "):\n";
            for (const auto& item : score)
                std::cout << "\t" << item.first << ": " << item.second << '\n';
            return true;
        }
        else {
            std::cout << COLORYELLOW << "No subjects yet\n" << COLORDEFAULT;
            return false;
        }
    }

public:
    student(
        unsigned id,
        const std::string& name,
        const std::string& surname,
        unsigned int educationYear,
        std::string group
    )
        : member(id, name, surname, educationYear),
        group(std::move(group))
    {
    }
    // constructor for copy class
    student(const student& other)
        : member(other),
          group(other.group),
          isGraduating(other.isGraduating),
          score(other.score)
    {
    }

    std::string getGroup() const { return group; }
    bool getIsGraduating() const { return isGraduating; }
    std::vector<std::pair<std::string, unsigned short int>> getScore() const { return score; }

    void setScore(const std::vector<std::pair<std::string, unsigned short int>>& newScore) { score = newScore; }

    // for print
    void printScore() const {
        showScores();
    }

    // subject is existing
    bool hasSubject(const std::string& subjectName) const {
        for (const auto& item : score)
            if (item.first == subjectName)
                return true;
        return false;
    }



    // work with subjects
    void addSubject() {
        showScores();

        std::string subject;
        unsigned short int mark;
        std::cout << "\nWhat subject to add: ";
        std::cin >> subject;
        std::cout << "Enter mark of " << subject << ": ";
        std::cin >> mark;

        try {
            if (hasSubject(subject))
                throw std::invalid_argument("Subject '" + subject + "' already exists");

            if (mark < 2 || mark > 5)
                throw std::out_of_range("Mark must be between 2 and 5");

            score.push_back({ subject, mark });
            std::cout << COLORGREEN << "Added: " << subject << " with mark " << mark << COLORDEFAULT << "\n";
        }
        catch (const std::exception& e) {
            std::cerr << COLORRED << "Error: " << e.what() << '\n' << COLORDEFAULT;
        }
    }
    void editMark() {
        if (!showScores())
            return; 

        std::string subject;
        unsigned short int newMark;
        std::cout << "\nWhat subject to edit: ";
        std::cin >> subject;
        std::cout << "Enter mark of " << subject << ": ";
        std::cin >> newMark;

        try {
            if (!hasSubject(subject))
                throw std::invalid_argument("Subject '" + subject + "' not found");

            if (newMark < 2 || newMark > 5)
                throw std::out_of_range("Mark must be between 2 and 5");

            for (auto& item : score) {
                if (item.first == subject) {
                    unsigned short int oldMark = item.second;
                    item.second = newMark;
                    std::cout << COLORGREEN << "Changed: " << subject << " from " << oldMark << " to " << newMark << COLORDEFAULT << "\n";
                    return;
                }
            }
        }
        catch (const std::exception& e) {
            std::cerr << COLORRED << "Error: " << e.what() << '\n' << COLORDEFAULT;
        }
    }
    void deleteScore() {
        if (!showScores())
            return;

        std::string subject;
        std::cout << "\nEnter subject to delete: ";
        std::cin >> subject;

        try {
            for (auto it = score.begin(); it != score.end(); ++it) {
                if (it->first == subject) {
                    score.erase(it);
                    std::cout << COLORGREEN << "Deleted: " << subject << COLORDEFAULT << "\n";
                    return;
                }
            }
            throw std::invalid_argument("Subject '" + subject + "' not found");
        }
        catch (const std::exception& e) {
            std::cerr << COLORRED << "Error: " << e.what() << '\n' << COLORDEFAULT;
        }
    }


    //work with sorting subjects
    void sortByName() {
        try {
            if (score.empty())
                throw std::invalid_argument{ "Not subjects yet" };

            char choose;
            std::cout << "Do you want to sort A-Z? \n(y/n | default: y): ";
            std::cin >> choose; CINCHAR;

            std::vector<std::pair<std::string, unsigned short int>> sortedScore = score;
            if(choose == 'n' || choose == 'N')
                std::sort(sortedScore.begin(), sortedScore.end(), [](const auto& a, const auto& b) { return a.first > b.first; });
            else
                std::sort(sortedScore.begin(), sortedScore.end(), [](const auto& a, const auto& b) { return a.first < b.first; });

            for (const auto& item : sortedScore)
                std::cout << "Subject: " << item.first << ": " << item.second << '\n';
        }
        catch (const std::exception& e) {
            std::cerr << COLORRED << "Error: " << e.what() << '\n' << COLORDEFAULT;
        }
    }
    void sortByMark() {
        try {
            if (score.empty())
                throw std::invalid_argument{ "Not subjects yet" };

            char choose;
            std::cout << "Do you want to sort from 5 to 2? \n(y/n | default: y): ";
            std::cin >> choose; CINCHAR;

            std::vector<std::pair<std::string, unsigned short int>> sortedScore = score;
            if (choose == 'n' || choose == 'N')
                std::sort(sortedScore.begin(), sortedScore.end(), [](const auto& a, const auto& b) { return a.second < b.second; });
            else
                std::sort(sortedScore.begin(), sortedScore.end(), [](const auto& a, const auto& b) { return a.second > b.second; });

            for (const auto& item : sortedScore)
                std::cout << "Subject: " << item.first << ": " << item.second << '\n';
        }
        catch (const std::exception& e) {
            std::cerr << COLORRED << "Error: " << e.what() << '\n' << COLORDEFAULT;
        }
    }

    // work with years and marks
    void nextYear(){
        if(!isGraduating){
            std::cout << "Student " << getName() << " end graduating at university " << getYearsInUniversity() << " years ago";
            setYearsInUniversity(getYearsInUniversity() + 1);
            return;
        }

        if(getYearsInUniversity() == 4){
            setYearsInUniversity(1);
            isGraduating = false;
            std::cout << "Student " << getName() << " ended study in university\n";
            score.clear();
            return;
        }

        setYearsInUniversity(getYearsInUniversity() + 1);
        std::cout << "Student " << getName() << " is now educating " << getYearsInUniversity() << " year\n";
    }
    void startSession(){
        nextYear();
        
        if(isGraduating){
            unsigned short int newMark;
            for(auto& item : score){
                while(true){
                    std::cout << "Enter new mark at the " << item.first << ": ";
                    std::cin >> newMark;
                    if (newMark < 2 || newMark > 5){
                        std::cerr << "Mark must be between 2 and 5\n";
                    }
                    else{
                        item.second = newMark;
                        break;
                    }
                }
            }
        std::cout << "All marks for this student changed\n\n";
        }
    }


    // print
    void print() const override{
        std::cout << "id: " << getId() << "\tStudent " << getName() << ' ' << getSurname() << "\tGroup: " << getGroup() << '\n';
    }
    void printAll() const override {
        std::cout << "\nStudent"
        << " id: " << getId() << " | " << getName() << ' ' << getSurname();
        if(isGraduating){
            std::cout << "\n\tGraduating year: " << getYearsInUniversity() << '\n';
            showScores();
        }else{
            std::cout << "\n\tStudent end studing at university " << getYearsInUniversity() << " years ago\n";
        }
    }
};

// menu
void studentSelfMenu(student student);
void studentMenu(student student);

// database
void registerStudentToDB(student& student, const std::string& login, const std::string& password);
student getStudentFromDB(const std::string& login, const std::string& password);
void updateStudentInDB(const student& student, const std::string& login, const std::string& password);