#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <limits>

#define COLORGREEN "\033[32m"
#define COLORYELLOW "\033[33m"
#define COLORRED "\033[31m"
#define COLORDEFAULT "\033[0m"


#undef max
#define CINCHAR std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n')

void wait();
void clearScreen();

// for Global session; not for professor or student .. etc.
extern unsigned long int GlobalSessionYear; // database
void nextGlobalSessionYear();
unsigned long int getGlobalSessionYear();