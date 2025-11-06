set(BUILD_VERSION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/build_number.txt")
set(FLASH_NUMBER_FILE "${CMAKE_CURRENT_SOURCE_DIR}/flashing_number.txt")
set(BUILD_HEADER_FILE "${CMAKE_CURRENT_BINARY_DIR}/utils/build_version.h")

# ------ BUILD COUNT LOGIC ------
# Read current build version
if(EXISTS "${BUILD_VERSION_FILE}")
    file(READ "${BUILD_VERSION_FILE}" BUILD_NUMBER)
    string(STRIP "${BUILD_NUMBER}" BUILD_NUMBER)
else()
    set(BUILD_NUMBER "0")
endif()

# ------ FLASH COUNT LOGIC ------
# Read current flash count
if(EXISTS "${FLASH_NUMBER_FILE}")
    file(READ "${FLASH_NUMBER_FILE}" FLASH_NUMBER)
    string(STRIP "${FLASH_NUMBER}" FLASH_NUMBER)
else()
    set(FLASH_NUMBER "0")
endif()

# Optionally, expose the header path as a variable
set(BUILD_VERSION_HEADER "${BUILD_HEADER_FILE}" PARENT_SCOPE)

set(BUILD_NUMBER "${BUILD_NUMBER}" PARENT_SCOPE)
set(FLASH_NUMBER "${FLASH_NUMBER}" PARENT_SCOPE)
