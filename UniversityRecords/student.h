#include "common.h"
#include "members.h"

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
    {
    }

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

    // for print
    void printScore() const {
        showScores();
    }

    bool hasSubject(const std::string& subjectName) const {
        for (const auto& item : score)
            if (item.first == subjectName)
                return true;
        return false;
    }

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

    void print() const override {
        std::cout << "id: " << getId() << " | " << getName() << ' ' << getSurname() << "\t Graduation year: " << getYearsInUniversity() << '\n';
        showScores();
    }
};

void studentMenu(student st);