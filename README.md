# UniversityRecords
  
Explanation of the project:  
Project on c++, using Docker, PostgreSQL, Cmake.  
University Records is a program that can be used in different universities.   
Main idea: like in my university first of all you need to create an admin accound with type: 'admin'. Then log in as admin.   
- As admin, user can create/redact all other users(professors and students), run tests and some other features.  
- As professor, user can read all info about him and redact all students in its group and start new year for all students in their group.  
- As student, user can read his marks and professors in his group.  

# To install and run
## Windows
1. Install VS Code or other redactor (optional)
2. Install Cmake
3. Install libpqxx from vcpkg in the same directory, where the project is located
4. Install docker
5. Use git clone *, then go to the path of the project in the cmd
6. In the /PATH/ of the project run "docker-compose up -d" (or to see more info "docker-compose up --build")
7. In cmd type "mkdir build" then go to the build folder 
9. There type "cmake .. -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake" (or other folder path to the vcpkg.cmake !This need to add libpqxx to the project)
10. Then "cmake --build ."
11. Then run "./Debug/UniversityRecords.exe" file 

## MacOs
1. Install VS Code or other redactor (optional)
2. Install Cmake
3. Install libpqxx with brew
4. Install docker
5. Use git clone *, then go to the path of the project in the terminal
6. In the /PATH/ of the project run "docker-compose up -d" (or to see more info "docker-compose up --build")
7. Create folder 'build' then in termenal go to folder "cd build"
8. Type "cmake .."
9. Then to compile write "cmake --build ."
10. To run the project just start ./UniversityRecords

# When first start the project
- Install program to see DB (recomendation: Dbeaver)
- Open and connect (login and password you can see in the docker-compose.yml file (DBName: UniversityRecords, DBPassword: 123))  
  
! (Project for qwinki resume)
