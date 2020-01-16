cmake_minimum_required(VERSION 3.12)

set(libraries_dir "${CMAKE_CURRENT_LIST_DIR}/../../../Libraries")
set(CMAKE_TOOLCHAIN_FILE "${libraries_dir}/Software/toolchain.cmake")

project("Beacon")

set(CMAKE_C_STANDARD "99")
enable_language(ASM)

set(linker_script "${libraries_dir}/Software/stm32l011f4.ld")
set(device_name "STM32L011xx")

set(asm_compile_options
	"-mcpu=cortex-m0plus"
	"-g3"
	"-c"
	"-x"
	"assembler-with-cpp"
	"--specs=nano.specs"
	"-mfloat-abi=soft"
	"-mthumb"
)

set(c_compile_options
	"-mcpu=cortex-m0plus"
	"-std=gnu99"
	"-g3"
	"-c"
	"-O0"
	"-ffunction-sections"
	"-fdata-sections"
	"-Wall"
	"-fstack-usage"
	"--specs=nano.specs"
	"-mfloat-abi=soft"
	"-mthumb"
)

set(compile_definitions "${device_name}")
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	list(APPEND compile_definitions "DEBUG" "_DEBUG")
else()
	list(APPEND compile_definitions "NDEBUG")
endif()

set(link_flags
	"-mcpu=cortex-m0plus \
	-T\"${linker_script}\" \
	--specs=nosys.specs -Wl,-Map=\"${PROJECT_NAME}.map\" \
	-Wl,--gc-sections \
	-static \
	--specs=nano.specs \
	-mfloat-abi=soft \
	-mthumb \
	-Wl,--start-group -lc -lm -Wl,--end-group"
)

# Compile options and definitions that should be set globally.
# For local to the project use `target_compile_definitions`
add_compile_options("$<$<COMPILE_LANGUAGE:C>:${c_compile_options}>")
add_compile_options("$<$<COMPILE_LANGUAGE:ASM>:${asm_compile_options}>")
add_compile_definitions("${compile_definitions}")

# Dependencies
add_subdirectory("${libraries_dir}/Software/CMSIS" "CMSIS")
add_subdirectory("${libraries_dir}/Software/STM32L0xx_HAL_Driver" "STM32L0xx_HAL")

add_subdirectory("${libraries_dir}/Software/printf" "printf")
target_compile_definitions("printf" PRIVATE "PRINTF_INCLUDE_CONFIG_H")

# Create an alias target `HAL` because nrf24 needs to have it defined before
add_library("HAL" ALIAS "STM32L0xx_HAL")
add_subdirectory("${libraries_dir}/Software/nrf24" "nrf24")

# Let the libraries know where they should look for their configs
target_include_directories("nrf24" PRIVATE "${CMAKE_CURRENT_LIST_DIR}/config")
target_include_directories("STM32L0xx_HAL" PRIVATE "${CMAKE_CURRENT_LIST_DIR}/config")
target_include_directories("printf" PRIVATE "${CMAKE_CURRENT_LIST_DIR}/config")

file(GLOB_RECURSE sources "src/*.[c|s|h]")
add_executable("${PROJECT_NAME}" "${sources}")
set_target_properties("${PROJECT_NAME}" PROPERTIES OUTPUT_NAME "program.elf")
target_link_libraries("${PROJECT_NAME}" "nrf24" "printf" "CMSIS" "STM32L0xx_HAL")
set_target_properties("${PROJECT_NAME}" PROPERTIES LINK_FLAGS "${link_flags}")
