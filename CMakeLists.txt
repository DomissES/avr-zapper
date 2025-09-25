cmake_minimum_required(VERSION 3.28)

##########------------------------------------------------------##########
##########              Project-specific Details                ##########
##########    Check these every time you start a new project    ##########
##########------------------------------------------------------##########
# set(PROG_TYPE avrdude)
set(MCU   atmega8a)
set(F_CPU 16000000)
#set(BAUD  9600)

# Include toolchain
include("cmake/avr-toolchain.cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)
# Core project settings
project (
    Zapper
    VERSION 1.0
    LANGUAGES ASM C

)
# Set build type 
# set(CMAKE_CONFIGURATION_TYPES Debug Release) #(this works only in multi config generators)
set(CMAKE_CONFIGURATION_TYPES Debug)
# Adding additional files generation
add_compile_options("-Wa,-ahls=$<CONFIG>/${PROJECT_NAME}.lst")
add_link_options("-Wl,-Map=$<CONFIG>/${PROJECT_NAME}.map")

message("Build type:  ${CMAKE_BUILD_TYPE}")
add_executable(${PROJECT_NAME})
# Setting file paths
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
set(OUTPUT_FULL_FILENAME_BASE    "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROJECT_NAME}")
message(STATUS "Output path: ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")

# Project setup
include(cmake/add_subdirs.cmake)

# Versioning
include(cmake/versioning.cmake)
get_version_from_git()
put_version_into_header()

# Doxygen target
include(cmake/doxygen.cmake)

# # Clang tidy target
# set(CMAKE_C_CLANG_TIDY "clang-tidy")
# set(CMAKE_CXX_CLANG_TIDY "clang-tidy")

# Compiling targets
add_custom_target(strip ALL
    COMMAND ${AVRSTRIP} "${OUTPUT_FULL_FILENAME_BASE}.elf" > NUL 2>&1
    DEPENDS ${PROJECT_NAME}
    VERBATIM
)
add_custom_target(hex ALL
    COMMAND ${OBJCOPY} -R .eeprom -O ihex "${OUTPUT_FULL_FILENAME_BASE}.elf" "${OUTPUT_FULL_FILENAME_BASE}.hex"
    DEPENDS strip
    VERBATIM
)

add_custom_target(eeprom ALL
   COMMAND ${OBJCOPY} -j .eeprom --change-section-lma .eeprom=0 -O ihex "${OUTPUT_FULL_FILENAME_BASE}.elf" "${RUNTIME_OUTPUT_DIRECTORY}.eeprom"
   DEPENDS strip
   VERBATIM
)

#add_custom_target(flash ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} -U flash:w:${PROJECT_NAME}.hex DEPENDS hex)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_FILES 
    "${OUTPUT_FULL_FILENAME_BASE}.hex;
    ${OUTPUT_FULL_FILENAME_BASE}.eeprom;
    ${OUTPUT_FULL_FILENAME_BASE}.lst;
    ${OUTPUT_FULL_FILENAME_BASE}.map"
)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES 
    "${OUTPUT_FULL_FILENAME_BASE}.hex;S
    ${OUTPUT_FULL_FILENAME_BASE}.eeprom;
    ${OUTPUT_FULL_FILENAME_BASE}.lst;
    ${OUTPUT_FULL_FILENAME_BASE}.map"
)