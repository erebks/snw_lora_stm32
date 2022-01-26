SET(CMAKE_SYSTEM_NAME      Generic)
SET(CMAKE_SYSTEM_VERSION   1)
SET(CMAKE_SYSTEM_PROCESSOR arm)
SET(CMAKE_CROSSCOMPILING   TRUE)

set(ARM_PATH "/opt/gcc-arm-none-eabi-10.3-2021.10")

include_directories(${ARM_PATH}/lib/gcc/arm-none-eabi/10.3.1/include)

SET(CMAKE_C_COMPILER   ${ARM_PATH}/bin/arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER ${ARM_PATH}/bin/arm-none-eabi-gcc)
SET(CMAKE_CXX_COMPILER ${ARM_PATH}/bin/arm-none-eabi-g++)

set(CMAKE_FIND_ROOT_PATH  ${ARM_PATH})

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(MCU "-mcpu=cortex-m0plus -mthumb")
set(COMMON_FLAGS "${MCU} -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra -g3" )

SET(LINKER_SCRIPT_MEM      ${PROJECT_SOURCE_DIR}/STM32L072CZTx_FLASH.ld)

UNSET(CMAKE_C_FLAGS          CACHE)
UNSET(CMAKE_EXE_LINKER_FLAGS CACHE)

set(ASM_OPTIONS            "-x assembler-with-cpp")
SET(CMAKE_ASM_FLAGS        "${COMMON_FLAGS} ${ASM_OPTIONS}")
set(CMAKE_C_FLAGS          "${COMMON_FLAGS} -std=gnu11" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS        "${COMMON_FLAGS} -std=gnu++11" CACHE STRING "" FORCE)
SET(CMAKE_EXE_LINKER_FLAGS "${COMMON_FLAGS} -T ${LINKER_SCRIPT_MEM} -Xlinker --gc-sections -Wl,-Map,\"${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map\" -Wl,--print-memory-usage --specs=nano.specs" CACHE STRING "" FORCE)

set_source_files_properties(${ASM_SOURCES} PROPERTIES COMPILE_FLAGS ${CMAKE_ASM_FLAGS})
