set(BUILD_VERSION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../build_version.txt")
set(FLASH_COUNT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../flash_count.txt")
set(BUILD_HEADER_FILE "${CMAKE_CURRENT_BINARY_DIR}/utils/build_version.h")

# Read current build version
if(EXISTS "${BUILD_VERSION_FILE}")
    file(READ "${BUILD_VERSION_FILE}" BUILD_VERSION)
    string(STRIP "${BUILD_VERSION}" BUILD_VERSION)
else()
    set(BUILD_VERSION "0")
endif()

# Increment build version
math(EXPR BUILD_VERSION_INC "${BUILD_VERSION} + 1")

# Write new build version back to file
file(WRITE "${BUILD_VERSION_FILE}" "${BUILD_VERSION_INC}")

# --- FLASH COUNT LOGIC ---
# Read current flash count
if(EXISTS "${FLASH_COUNT_FILE}")
    file(READ "${FLASH_COUNT_FILE}" FLASH_COUNT)
    string(STRIP "${FLASH_COUNT}" FLASH_COUNT)
else()
    set(FLASH_COUNT "0")
endif()

# Optionally, expose the header path as a variable
set(BUILD_VERSION_HEADER "${BUILD_HEADER_FILE}" PARENT_SCOPE)