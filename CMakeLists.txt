cmake_minimum_required(VERSION 3.20.2)
project(FinalProject C CXX)
set(PROJECT_VERSION 0.5.8)

option(ENABLE_ADDRSAN "Enable the address sanitizer" OFF)
option(ENABLE_UBSAN "Enable the ub sanitizer" OFF)
option(ENABLE_TSAN "Enable the thread data race sanitizer" OFF)
option(PART_B "Build for part B version of the assignment" ON)
option(SOURCE_RELATIVE "Use the cmake source directory as a base" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
message("${CMAKE_C_FLAGS}")
add_compile_options(-mieee-fp)
#set(CMAKE_C_FLAGS -mieee-fp)hz
#add_definitions(-DUSEVFORK -DPOSIX_MT)
add_compile_definitions(USEVFORK POSIX_MT RANDOMSEEDTIME)
if (PART_B)
    add_compile_definitions(PART_B)
endif ()
if (SOURCE_RELATIVE)
    add_compile_definitions(SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\")
    add_compile_definitions(BUILD_DIR=\"${CMAKE_BINARY_DIR}\")
endif ()
message("${CMAKE_C_FLAGS}")

find_package(Threads)

add_subdirectory(lib/blt)

include_directories(include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/lilgp/kernel/)
file(GLOB_RECURSE PROJECT_BUILD_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/program/*.cpp")
file(GLOB_RECURSE PROJECT_BUILD_FILES_C "${CMAKE_CURRENT_SOURCE_DIR}/src/program/*.c")
set(LILGP_BUILD_FILES main.c gp.c eval.c tree.c change.c crossovr.c reproduc.c
        mutate.c select.c tournmnt.c bstworst.c fitness.c genspace.c
        exch.c populate.c ephem.c ckpoint.c event.c pretty.c individ.c
        params.c random.c memory.c output.c boltzman.c sigma.c fsetupdate.c)
list(TRANSFORM LILGP_BUILD_FILES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/lib/lilgp/kernel/)

add_executable(FinalProject ${PROJECT_BUILD_FILES} ${PROJECT_BUILD_FILES_C} ${LILGP_BUILD_FILES})

target_link_libraries(FinalProject PUBLIC BLT)

target_compile_options(FinalProject PRIVATE -Wall -Wextra -Wpedantic -Wno-comment)
target_link_options(FinalProject PRIVATE -Wall -Wextra -Wpedantic -Wno-comment)
target_link_libraries(FinalProject PUBLIC m)
target_link_libraries(FinalProject PUBLIC pthread)

if (${ENABLE_ADDRSAN} MATCHES ON)
    target_compile_options(FinalProject PRIVATE -fsanitize=address)
    target_link_options(FinalProject PRIVATE -fsanitize=address)
endif ()

if (${ENABLE_UBSAN} MATCHES ON)
    target_compile_options(FinalProject PRIVATE -fsanitize=undefined)
    target_link_options(FinalProject PRIVATE -fsanitize=undefined)
endif ()

if (${ENABLE_TSAN} MATCHES ON)
    target_compile_options(FinalProject PRIVATE -fsanitize=thread)
    target_link_options(FinalProject PRIVATE -fsanitize=thread)
endif ()

project(FinalProject_Runner C CXX)
file(GLOB_RECURSE Runner_PROJECT_BUILD_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/runner/*.cpp")

add_executable(FinalProject_Runner ${Runner_PROJECT_BUILD_FILES})

target_link_libraries(FinalProject_Runner PUBLIC BLT)
target_link_libraries(FinalProject_Runner PUBLIC m)
target_link_libraries(FinalProject_Runner PUBLIC pthread)
target_include_directories(FinalProject_Runner PUBLIC Runner)
target_compile_options(FinalProject_Runner PRIVATE -Wall -Wextra -Wpedantic -Wno-comment)
target_link_options(FinalProject_Runner PRIVATE -Wall -Wextra -Wpedantic -Wno-comment)
add_dependencies(FinalProject_Runner FinalProject)

message("Checking for sanatizers")
if (${ENABLE_ADDRSAN} MATCHES ON)
    target_compile_options(FinalProject_Runner PRIVATE -fsanitize=address)
    target_link_options(FinalProject_Runner PRIVATE -fsanitize=address)
    message(STATUS "Enabled addrsan")
endif ()

if (${ENABLE_UBSAN} MATCHES ON)
    target_compile_options(FinalProject_Runner PRIVATE -fsanitize=undefined)
    target_link_options(FinalProject_Runner PRIVATE -fsanitize=undefined)
    message(STATUS "Enabled UBsan")
endif ()

if (${ENABLE_TSAN} MATCHES ON)
    target_compile_options(FinalProject_Runner PRIVATE -fsanitize=thread)
    target_link_options(FinalProject_Runner PRIVATE -fsanitize=thread)
    message(STATUS "Enabled threadsan")
endif ()
