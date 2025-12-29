#include "common.h"

void clearScreen() {
#ifdef _WIN32
    // windows
    system("cls");
#else
    // linux, macos
    system("clear");
#endif
}

void wait() {
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}
