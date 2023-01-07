cmake_minimum_required(VERSION 3.5)

project(Hakkun LANGUAGES C CXX ASM)

if(NOT DEFINED ENV{DEVKITARM})
    message(FATAL_ERROR "please set DEVKITARM")
endif()

if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Data/musl)
    message(FATAL_ERROR "musl not found")
endif()

find_program(EXHEADER_CLI NAMES ExheaderCli PATHS Tools/ExheaderCli/Build/ REQUIRED NO_DEFAULT_PATH)

execute_process(COMMAND ${EXHEADER_CLI} printCodeOffset=true ${CMAKE_CURRENT_SOURCE_DIR}/Data/exheader.bin OUTPUT_VARIABLE CODE_OFFSET)

file(READ ${CMAKE_CURRENT_SOURCE_DIR}/Data/LinkerTemplate.ld.in LINKER_TEMPLATE)
string(REPLACE "CODE_OFFSET" "${CODE_OFFSET}" LINKER_SCRIPT "${LINKER_TEMPLATE}")
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/Linker.ld "${LINKER_SCRIPT}")

set(SYMS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Symbols)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXECUTABLE_SUFFIX ".axf")

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_ASM_COMPILER clang)

set(CMAKE_CXX_STANDARD 20)

set(MUSL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Data/musl/)

set(INCLUDES "-isystem ${MUSL_DIR}/include -isystem ${MUSL_DIR}/obj/include -isystem ${MUSL_DIR}/arch/arm -I ${CMAKE_CURRENT_SOURCE_DIR}/Include")
set(LIBS "${MUSL_DIR}/lib/libc.a")
set(ARCH_FLAGS "-target armv6k-none-eabi -mtune=mpcore -mfloat-abi=hard -mtp=soft -mfpu=vfpv2")

set(CMAKE_ASM_FLAGS "${ARCH_FLAGS} -x assembler-with-cpp")
set(CMAKE_CXX_FLAGS "${ARCH_FLAGS} ${INCLUDES} \
                    -g -O3 \
                    -fno-exceptions \
                    ")

set(CMAKE_EXE_LINKER_FLAGS "-nodefaultlibs ${ARCH_FLAGS} ${LIBS} -T ${CMAKE_CURRENT_BINARY_DIR}/Linker.ld -T ${CMAKE_CURRENT_BINARY_DIR}/Symbols.ld -Wl,--no-demangle -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.map")

file(GLOB_RECURSE ASMSOURCEFILES "${CMAKE_SOURCE_DIR}/Source/*.S")
file(GLOB_RECURSE CSOURCEFILES "${CMAKE_SOURCE_DIR}/Source/*.c")
file(GLOB_RECURSE CXXSOURCEFILES "${CMAKE_SOURCE_DIR}/Source/*.cpp")

add_executable(Hakkun ${CSOURCEFILES} ${CXXSOURCEFILES} ${ASMSOURCEFILES})

# CC="clang" CFLAGS="-target armv6k-none-eabi -mtune=mpcore -mfloat-abi=hard -mtp=soft -mfpu=vfpv2" LDFLAGS="-Wl,--unresolved-symbols=ignore-all" AR="llvm-ar" RANLIB="llvm-ranlib" LIBCC=" " ./configure --target=armv6k-none-eabi