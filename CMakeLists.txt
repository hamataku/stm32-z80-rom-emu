cmake_minimum_required(VERSION 3.10)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

SET(CMAKE_C_COMPILER arm-none-eabi-gcc CACHE STRING "arm-none-eabi-gcc" FORCE)
SET(CMAKE_CXX_COMPILER arm-none-eabi-g++ CACHE STRING "arm-none-eabi-g++" FORCE)
SET(AS arm-none-eabi-gcc  CACHE STRING "arm-none-eabi-as")
SET(CMAKE_AR arm-none-eabi-ar CACHE STRING "arm-none-eabi-ar" FORCE)
SET(CMAKE_LD arm-none-eabi-ld CACHE STRING "arm-none-eabi-ld" FORCE)
SET(NM arm-none-eabi-nm CACHE STRING "arm-none-eabi-nm" FORCE)
SET(OBJCOPY arm-none-eabi-objcopy CACHE STRING "arm-none-eabi-objcopy" FORCE)
SET(OBJDUMP arm-none-eabi-objdump CACHE STRING "arm-none-eabi-objdump" FORCE)
SET(READELF arm-none-eabi-readelf CACHE STRING "arm-none-eabi-readelf" FORCE)
SET(CMAKE_RANLIB arm-none-eabi-ranlib CACHE STRING "arm-none-eabi-ranlib" FORCE)


enable_language(C)
enable_language(CXX)
enable_language(ASM)

# project name and language to use
project(f446-firmware CXX)
string(ASCII 27 Esc)
if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Release)
endif()


# set compiler option
set(COMMON_COMPILER_FLAGS "-DUSE_HAL_DRIVER -DSTM32F446xx -mcpu=cortex-m4 -mabi=aapcs -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mthumb -Wall -Wextra -fshort-wchar -fdata-sections -ffunction-sections -Wno-missing-field-initializers")
set(CMAKE_C_FLAGS "${COMMON_COMPILER_FLAGS} -std=gnu99")
set(CMAKE_C_FLAGS_DEBUG "-O2 -g")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS "${COMMON_COMPILER_FLAGS} -D_GLIBCXX_USE_C99 -Wno-missing-field-initializers -std=gnu++17")
set(CMAKE_CXX_FLAGS_DEBUG "-O2 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_ASM_FLAGS "${COMMON_COMPILER_FLAGS} -x assembler-with-cpp")
set(CMAKE_ASM_FLAGS_DEBUG "-O2 -g")
set(CMAKE_ASM_FLAGS_RELEASE "-O2")


# Echo compile flags
message("Build type:\n\t${CMAKE_BUILD_TYPE}")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Compile flags:\n\t${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Compile flags:\n\t${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
else()
    message("Compile flags:\n\t${CMAKE_CXX_FLAGS}")
endif()

set(CMAKE_EXE_LINKER_FLAGS "-mcpu=cortex-m4 -DSTM32F446xx -mthumb -specs=nano.specs -T ${CMAKE_SOURCE_DIR}/STM32F446RETx_FLASH.ld -lc -lm -Wl,--gc-sections -Wl,--no-wchar-size-warning -u _printf_float -Wl,-Map=${CMAKE_PROJECT_NAME}.map -Wl,--print-memory-usage")

# includes
include_directories("Core/Inc" "z80" "Drivers/CMSIS/Device/ST/STM32F4xx/Include" "Drivers/CMSIS/Include" "Drivers/STM32F4xx_HAL_Driver/Inc" "Drivers/STM32F4xx_HAL_Driver/Inc/Legacy")

# sources
file(GLOB PERIPH_LIB_SOURCE "Drivers/STM32F4xx_HAL_Driver/Src/*.c" "*.s")
file(GLOB USERS_SOURCE "Core/Src/*.cpp" "Core/Src/*.c" "z80/*.c")
# list(REMOVE_ITEM USERS_SOURCE "${CMAKE_SOURCE_DIR}/Core/Src/main.c")
add_executable(${PROJECT_NAME}.elf  ${PERIPH_LIB_SOURCE} ${USERS_SOURCE})


add_custom_target(${PROJECT_NAME}.hex
        COMMAND arm-none-eabi-objcopy --output-format=ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex DEPENDS ${PROJECT_NAME}.elf)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${PROJECT_NAME}.hex)

if(EXISTS /proc/sys/fs/binfmt_misc/WSLInterop)
   # WINDOWS(WSL)
   add_custom_target(asm_write COMMAND python3 ../z80/hex2source.py COMMAND make COMMAND /mnt/c/Program\ Files/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI.exe -c port=SWD -d ./${PROJECT_NAME}.elf -s )
   add_custom_target(write COMMAND make COMMAND /mnt/c/Program\ Files/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI.exe -c port=SWD -d ./${PROJECT_NAME}.elf -s )
elseif(EXISTS /Applications/STMicroelectronics)
   # MAC
   add_custom_target(asm_write COMMAND python3 ../z80/hex2source.py COMMAND make COMMAND /Applications/STMicroelectronics/STM32Cube/STM32CubeProgrammer/STM32CubeProgrammer.app/Contents/MacOs/bin/STM32_Programmer_CLI -c port=SWD -d ./${PROJECT_NAME}.elf -s )
   add_custom_target(write COMMAND make COMMAND /Applications/STMicroelectronics/STM32Cube/STM32CubeProgrammer/STM32CubeProgrammer.app/Contents/MacOs/bin/STM32_Programmer_CLI -c port=SWD -d ./${PROJECT_NAME}.elf -s )
else()
   # Native LINUX
   add_custom_target(asm_write COMMAND python3 ../z80/hex2source.py COMMAND make COMMAND /usr/local/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer.sh -c port=SWD -d ./${PROJECT_NAME}.elf -s )
   add_custom_target(write COMMAND make COMMAND /usr/local/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer.sh -c port=SWD -d ./${PROJECT_NAME}.elf -s )
endif()