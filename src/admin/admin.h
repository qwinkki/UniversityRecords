#pragma once

#include "common/common.h"
#include "professor/professor.h"
#include "student/student.h"

// menu
void adminMenu(std::string& login);
void createUser();

// database
bool getAdminFromDB(std::string& login, std::string& password);
void getAllUsersFromDB();

void startEditingProfessor(int professorId);
void startEditingStudent(int studentId);

// tests
void runTests();

// extensions
void checkExtensionsAdmin();