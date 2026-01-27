#pragma once

#include "common/common.h"
#include "professor/professor.h"
#include "student/student.h"

// menu
void adminMenu();

// database
bool getAdminFromDB(std::string& login, std::string& password);
void getAllUsersFromDB();

void startEditingProfessor(int professorId);
void startEditingStudent(int studentId);

// tests
void runTests();