# CMakeLists.txt for TM4C129XL project

cmake_minimum_required(VERSION 3.15)

# Project name
project(tm4c129xl_project C ASM)

# Set language standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Specify the cross compiler
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Adjust these paths to match your ARM GCC installation
set(TOOLCHAIN_PREFIX arm-none-eabi-)
set(TOOLCHAIN_PATH "/usr/bin") # Adjust to your ARM GCC installation path

# Set compiler tools
set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_OBJCOPY ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_SIZE ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}size)

# MCU specific flags
set(CPU_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16")
set(CMAKE_C_FLAGS "${CPU_FLAGS} -Wall -Wextra -Werror -ffunction-sections -fdata-sections -fno-strict-aliasing")
set(CMAKE_ASM_FLAGS "${CPU_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CPU_FLAGS} -Wl,--gc-sections -T${CMAKE_SOURCE_DIR}/linker/tm4c1294xl.ld --specs=nano.specs --specs=nosys.specs")

# Debug flags
set(CMAKE_C_FLAGS_DEBUG "-O0 -g3")
set(CMAKE_C_FLAGS_RELEASE "-O3 -g0")

# Set the paths for the source, include, and third-party directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_SOURCE_DIR}/inc)
set(CFG_DIR ${CMAKE_SOURCE_DIR}/cfg)
set(CMSIS_DIR ${CMAKE_SOURCE_DIR}/third_party/CMSIS/Core)
set(TIVA_DIR ${CMAKE_SOURCE_DIR}/third_party/TI)
set(DRIVERLIB_DIR ${TIVA_DIR}/driverlib)
set(GRLIB_DIR ${TIVA_DIR}/grlib)
set(UCOS_DIR ${CMAKE_SOURCE_DIR}/third_party/uC-OS2)
set(UCOS_SRC_DIR ${UCOS_DIR}/uC-OS2)
set(UCOS_CPU_DIR ${UCOS_DIR}/uC-CPU)
set(UCOS_LIB_DIR ${UCOS_DIR}/uC-LIB)
set(LVGL_DIR ${CMAKE_SOURCE_DIR}/third_party/lvgl/src)


# Include directories
include_directories(
    ${INC_DIR}
    ${CFG_DIR}
)

# add executables 
file(GLOB SRC_FILES 
    "${SRC_DIR}/*.c"
    "${CFG_DIR}/*.c"
)

# Define library source files
file(GLOB_RECURSE LIB_SOURCES 
    #"${CMSIS_DIR}/Source/*.c"
    "${DRIVERLIB_DIR}/*.c" 
)

# Define library source files
file(GLOB_RECURSE LVGL_SOURCES 
    "${LVGL_DIR}/*.c"
)


# define the uC-OS2 source files
file(GLOB_RECURSE  UCOS_SOURCES
    "${UCOS_DIR}/*os_dbg.c"
    "${UCOS_DIR}/*os_core.c"
    "${UCOS_DIR}/*os_flag.c"
    "${UCOS_DIR}/*os_mbox.c"
    "${UCOS_DIR}/*os_mem.c"
    "${UCOS_DIR}/*os_mutex.c"
    "${UCOS_DIR}/*os_q.c"
    "${UCOS_DIR}/*os_sem.c"
    "${UCOS_DIR}/*os_task.c"
    "${UCOS_DIR}/*os_time.c"
    "${UCOS_DIR}/*os_tmr.c"
    "${UCOS_DIR}/*os_cpu_c.c"
    "${UCOS_DIR}/*cpu_c.c"
    "${UCOS_DIR}/*cpu_core.c"
    "${UCOS_DIR}/*lib_ascii.c"
    "${UCOS_DIR}/*lib_math.c"
    "${UCOS_DIR}/*lib_mem.c"
    "${UCOS_DIR}/*lib_str.c"
    "${UCOS_DIR}/*os_cpu_a.s"
    "${UCOS_DIR}/*cpu_a.s"
)

# Disable warnings for library files
foreach(SOURCE_FILE ${LIB_SOURCES})
    set_source_files_properties(${SOURCE_FILE} PROPERTIES COMPILE_FLAGS "-w")
endforeach()

# Disable warnings for library files
foreach(SOURCE_FILE ${UCOS_SOURCES})
    set_source_files_properties(${SOURCE_FILE} PROPERTIES COMPILE_FLAGS "-w")
endforeach()

# Disable warnings for library files
foreach(SOURCE_FILE ${LVGL_SOURCES})
    set_source_files_properties(${SOURCE_FILE} PROPERTIES COMPILE_FLAGS "-w")
endforeach()

# Set the compile definitions
add_compile_definitions(
    gcc
)

# Create executable
add_executable(${PROJECT_NAME}.elf ${SRC_FILES} ${LIB_SOURCES} ${UCOS_SOURCES} ${LVGL_SOURCES})

# Add include directories
target_include_directories(${PROJECT_NAME}.elf PRIVATE
    ${CMSIS_DIR}/Include
    ${TIVA_DIR}
    ${TIVA_DIR}/inc
    ${GRLIB_DIR}
    ${UCOS_SRC_DIR}/Ports/ARM-Cortex-M/ARMv7-M
    ${UCOS_SRC_DIR}/Ports/ARM-Cortex-M/ARMv7-M/GNU
    ${UCOS_SRC_DIR}/Source
    ${UCOS_CPU_DIR}
    ${UCOS_CPU_DIR}/ARM-Cortex-M/ARMv7-M/GNU
    ${UCOS_LIB_DIR}
    ${UCOS_LIB_DIR}/Ports/ARM-Cortex-M4/GNU
    ${LVGL_DIR}
)

# Print size of the binary
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_SIZE} --format=berkeley $<TARGET_FILE:${PROJECT_NAME}.elf>
)

# Create binary
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_NAME}.bin
    COMMENT "Creating binary ${PROJECT_NAME}.bin"
)

# Create hex file
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_NAME}.hex
    COMMENT "Creating hex file ${PROJECT_NAME}.hex"
)