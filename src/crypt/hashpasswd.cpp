#include "hashpasswd.h"

void hashPasswdSHA512(std::string& password){
    std::string salt = "$6$52UniversityRecords67";

    char* hash = crypt(password.c_str(), salt.c_str());

    if (hash) {
        password = hash;
    } else {
        throw "Error hashing password";
    }
}