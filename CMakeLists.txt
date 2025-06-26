cmake_minimum_required(VERSION 3.15)
project(numen
    VERSION 1.0.0
    DESCRIPTION "Numen - A high-performance C math library"
    LANGUAGES C
)

# Configuration options
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_STATIC_LIBS "Build static libraries" ON)
option(BUILD_TESTING "Build tests" ON)
option(BUILD_DOCS "Build documentation" ON)
option(INSTALL_SYSTEM_WIDE "Install system-wide" ON)

# Compiler settings for C
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -pedantic -Werror=implicit-function-declaration)
endif()

# Library setup
set(LIB_SOURCES
    src/utils.c
    src/vector.c
)
include_directories(include)

# Shared library
if(BUILD_SHARED_LIBS)
    add_library(numen_shared SHARED ${LIB_SOURCES})
    set_target_properties(numen_shared PROPERTIES
        OUTPUT_NAME "numen"
        VERSION ${PROJECT_VERSION}
        SOVERSION 1
    )
    target_link_libraries(numen_shared PUBLIC m)
endif()

# Static library
if(BUILD_STATIC_LIBS)
    add_library(numen_static STATIC ${LIB_SOURCES})
    set_target_properties(numen_static PROPERTIES
        OUTPUT_NAME "numen"
    )
    target_link_libraries(numen_static PUBLIC m)

    if(WIN32)
        set_target_properties(numen_static PROPERTIES OUTPUT_NAME "numen_s")
    endif()
endif()

# Installation
if(INSTALL_SYSTEM_WIDE)
    include(GNUInstallDirs)

    install(TARGETS numen_shared numen_static
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/numen)
endif()

# Testing with Unity
if(BUILD_TESTING)
    enable_testing()

    include(FetchContent)
    FetchContent_Declare(
        unity
        GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
        GIT_TAG v2.6.1
    )
    FetchContent_MakeAvailable(unity)

    set(TEST_SOURCES
        tests/utils_test.c
    )
    add_executable(numen_tests ${TEST_SOURCES})

    if(BUILD_SHARED_LIBS)
        target_link_libraries(numen_tests PRIVATE numen_shared)
    else()
        target_link_libraries(numen_tests PRIVATE numen_static)
    endif()

    target_link_libraries(numen_tests PRIVATE unity)
    add_test(NAME numen_tests COMMAND numen_tests)
endif()

# Documentation with Doxygen
if(BUILD_DOCS)
    find_package(Doxygen QUIET)
    if(DOXYGEN_FOUND)
        find_program(DOT_EXE dot)
        if(NOT DOT_EXE)
            message(WARNING "dot not found - diagrams will be disabled")
        endif()

        configure_file(
            ${CMAKE_SOURCE_DIR}/Doxyfile.in
            ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            @ONLY
        )

        add_custom_target(docs
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM
        )
    else()
        message(WARNING "Doxygen not found - documentation will not be generated")
    endif()
endif()
