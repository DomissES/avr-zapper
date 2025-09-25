set(CMAKE_SYSTEM_NAME                   Generic)
set(CMAKE_SYSTEM_PROCESSOR              avr)
set(CMAKE_TRY_COMPILE_TARGET_TYPE       STATIC_LIBRARY)

# Set toolchain path
set(TOOLCHAIN_DIR   "I:/Sw_develop/avr/avr8-gnu-toolchain")
set(SPECS_PATH      "${TOOLCHAIN_DIR}/../packs/ATmega_DFP/2.2.509/gcc/dev/${MCU}")
# Sets the compiler
set(CMAKE_ASM_COMPILER   "${TOOLCHAIN_DIR}/bin/avr-gcc.exe")
set(CMAKE_C_COMPILER   "${TOOLCHAIN_DIR}/bin/avr-gcc.exe")
# set(CMAKE_C_LINK_EXECUTABLE   "${TOOLCHAIN_DIR}/bin/avr-ld.exe")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_DIR}/bin/avr-g++.exe")

# program names
set(AVRCPP   avr-g++)
set(AVRC     avr-gcc)
set(AVRSTRIP avr-strip)
set(OBJCOPY  avr-objcopy)
set(OBJDUMP  avr-objdump)
set(AVRSIZE  avr-size)
set(AVRDUDE  avrdude)

set(CMAKE_FIND_ROOT_PATH_PROGRAM        NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY   ONLY
    "${TOOLCHAIN_DIR}/avr/include"
    "${TOOLCHAIN_DIR}/lib/gcc/avr/7.3.0/include"
    "${TOOLCHAIN_DIR}/lib/gcc/avr/7.3.0/include-fixed"
)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE  ONLY)

add_compile_options(
    "-mmcu=${MCU}"
    $<$<COMPILE_LANGUAGE:C>:-Wall>
    $<$<COMPILE_LANGUAGE:C>:-Wextra>
    # $<$<COMPILE_LANGUAGE:C>:-v> # verbose, not needed
    $<$<COMPILE_LANGUAGE:C>:-std=gnu99>
    $<$<COMPILE_LANGUAGE:C>:-funsigned-char>
    $<$<COMPILE_LANGUAGE:C>:-funsigned-bitfields>
    $<$<COMPILE_LANGUAGE:C>:-fpack-struct>
    $<$<COMPILE_LANGUAGE:C>:-fshort-enums>
    $<$<COMPILE_LANGUAGE:C>:-ffunction-sections>
    $<$<COMPILE_LANGUAGE:C>:-fdata-sections>
    "$<$<COMPILE_LANGUAGE:C>:-B ${SPECS_PATH}>"

    "$<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp>"
    "$<$<COMPILE_LANGUAGE:ASM>:-mmcu=${MCU}>"
    "$<$<COMPILE_LANGUAGE:ASM>:-Wa,-gdwarf2>"
    "$<$<COMPILE_LANGUAGE:ASM>:-Wa,-gstabs>"
    "$<$<COMPILE_LANGUAGE:ASM>:-B ${SPECS_PATH}>"

    $<$<CONFIG:DEBUG>:-O0>
    $<$<CONFIG:DEBUG>:-g3>
    $<$<CONFIG:DEBUG>:-ggdb>
    $<$<CONFIG:DEBUG>:-gdwarf>
    $<$<CONFIG:RELEASE>:-Os>
)

add_compile_definitions(
    F_CPU=${F_CPU} 
)

add_link_options(
    "-Wl,--gc-sections"
    "-Wl,--print-memory-usage"
    "-mmcu=${MCU}"
    "-B ${SPECS_PATH}"
)

link_libraries(
    "-Wl,--start-group"
    "-lc"
    "-lm"
    "-Wl,--end-group"
)

set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 99)
set(CMAKE_EXECUTABLE_SUFFIX .elf)
set(CMAKE_EXECUTABLE_SUFFIX_C .elf)

# set(CMAKE_ASM_FLAGS "-MMD -MP -MF $(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -Wa,-g")

