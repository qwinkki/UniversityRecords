cmake_minimum_required(VERSION 3.20)
project(UniversityRecords)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Vcpkg
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
    CACHE STRING "Vcpkg toolchain file")

find_package(libpqxx CONFIG REQUIRED)

add_executable(UniversityRecords
    src/main.cpp
    src/common/common.cpp
    src/database/database.cpp
    src/professor/professor.cpp
    src/student/student.cpp
)

target_include_directories(UniversityRecords PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(UniversityRecords PRIVATE libpqxx::pqxx)