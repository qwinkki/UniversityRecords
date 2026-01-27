#include "common.h"

void cinChar() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void clearScreen() {
#ifdef _WIN32
    // windows
    system("cls");
#else
    // macos
    system("clear");
#endif
}

void wait() {
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}