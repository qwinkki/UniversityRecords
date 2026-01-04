cmake_minimum_required(VERSION 3.20)
project(UniversityRecords)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Executable ---
add_executable(UniversityRecords
    src/main.cpp
    src/common/common.cpp
    src/database/database.cpp
    src/professor/professor.cpp
    src/student/student.cpp
)

# --- Cross-platform libpqxx setup ---
if(WIN32)
    # Windows + vcpkg
    if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "")
    endif()
    find_package(libpqxx CONFIG REQUIRED)

    target_include_directories(UniversityRecords PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

    target_link_libraries(UniversityRecords PRIVATE libpqxx::pqxx)
elseif(APPLE)
    # macOS + Homebrew
    set(LIBPQXX_INCLUDE_DIR /opt/homebrew/Cellar/libpqxx/7.10.4/include)
    set(LIBPQXX_LIBRARY     /opt/homebrew/Cellar/libpqxx/7.10.4/lib/libpqxx.dylib)
    set(LIBPQ_INCLUDE_DIR   /opt/homebrew/opt/libpq/include)
    set(LIBPQ_LIBRARY       /opt/homebrew/opt/libpq/lib/libpq.dylib)

    target_include_directories(UniversityRecords PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${LIBPQXX_INCLUDE_DIR}
        ${LIBPQ_INCLUDE_DIR}
    )

    target_link_libraries(UniversityRecords PRIVATE
        ${LIBPQXX_LIBRARY}
        ${LIBPQ_LIBRARY}
    )
endif()
