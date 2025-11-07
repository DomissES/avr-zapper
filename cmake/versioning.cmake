
# from:
# https://dev.to/khozaei/automating-semver-with-git-and-cmake-2hji

# CHanges:
# Due to the git workflow change, git describe shall always return a tag name, number of commits since tag, and short hash
# e.g. v0.2.0-1-gd65df0b

function(get_version_from_git)
    find_package(Git QUIET)
    if(NOT Git_FOUND)
        message(WARNING "Git not found")
        return()
    endif()

    # Get tag
    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_TAG
        OUTPUT_STRIP_TRAILING_WHITESPACE
        RESULT_VARIABLE GIT_RESULT
    )

    if(NOT GIT_RESULT EQUAL 0)
        message(WARNING "Failed to get git tag")
    endif()

    # Since getting regex in CMake is tricky (doesn't work for git hash) use function below
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short=7 HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_SHORT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    set(GIT_COMMIT_SHORT_HASH "${GIT_COMMIT_SHORT_HASH}" PARENT_SCOPE)

    # Get version numbers from tag
    string(REGEX MATCH "[0-9]+\.[0-9]+\.[0-9]+" CLEAN_TAG "${GIT_TAG}")

    if(CLEAN_TAG MATCHES "^([0-9]+)\\.([0-9]+)\\.([0-9]+)(-.*)?$")

        set(PROJECT_VERSION_MAJOR ${CMAKE_MATCH_1})
        set(PROJECT_VERSION_MAJOR ${CMAKE_MATCH_1} PARENT_SCOPE)
        set(PROJECT_VERSION_MINOR ${CMAKE_MATCH_2})
        set(PROJECT_VERSION_MINOR ${CMAKE_MATCH_2} PARENT_SCOPE)
        set(PROJECT_VERSION_PATCH ${CMAKE_MATCH_3})
        set(PROJECT_VERSION_PATCH ${CMAKE_MATCH_3} PARENT_SCOPE)

    else()
        message(STATUS "There is no valid git tag found. Using default version 0.0.0")

        set(PROJECT_VERSION_MAJOR 0)
        set(PROJECT_VERSION_MAJOR 0 PARENT_SCOPE)
        set(PROJECT_VERSION_MINOR 0)
        set(PROJECT_VERSION_MINOR 0 PARENT_SCOPE)
        set(PROJECT_VERSION_PATCH 0)
        set(PROJECT_VERSION_PATCH 0 PARENT_SCOPE)
    endif()

    set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}") 
    set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}" PARENT_SCOPE)


    # print build messages
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND echo "Build type: ${CMAKE_BUILD_TYPE}"
        # COMMAND echo "Git describe: ${GIT_TAG}"
        COMMAND echo "Project version: ${PROJECT_VERSION}"
        COMMAND echo "Git commit hash: ${GIT_COMMIT_SHORT_HASH}"
    )

endfunction()

function(put_version_into_header)

    option(ENABLE_BUILD_VERSION "Enable generating version.h" ON)

    if(ENABLE_BUILD_VERSION)

        option(ENABLE_BUILD_NUMBER "Enable including build_version.cmake" ON)

        if(ENABLE_BUILD_NUMBER)
        include("${CMAKE_SOURCE_DIR}/cmake/build_version.cmake")
            add_custom_command(
                TARGET ${PROJECT_NAME}
                POST_BUILD
                COMMAND python.exe tools/increase_build_number.py
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                DEPENDS hex
            )
        else()
            set(BUILD_NUMBER 0)
            set(FLASH_NUMBER 0)
        endif()

        add_custom_command(
            TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND echo "Build number: ${BUILD_NUMBER}"
            COMMAND echo "Flash number: ${FLASH_NUMBER}"
        )

        # Prepare file for version number and build number
        configure_file(
            "${CMAKE_SOURCE_DIR}/source/utils/version.h.in"
            "${CMAKE_BINARY_DIR}/version.h"
            @ONLY
        )
    endif()

endfunction()