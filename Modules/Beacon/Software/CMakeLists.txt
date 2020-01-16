cmake_minimum_required(VERSION 3.12)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/../../../Libraries/Software/toolchain.cmake")

project("cmake-stm32")

set(CMAKE_C_STANDARD "99")
enable_language(ASM)

file(GLOB_RECURSE sources "src/*.[c|s|h]")

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../../../Libraries/Software/nrf24" "nrf24")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../../../Libraries/Software/printf" "printf")

target_include_directories("nrf24" PRIVATE "${CMAKE_CURRENT_LIST_DIR}/src/config")

add_executable("${PROJECT_NAME}" ${sources})
set_target_properties("${PROJECT_NAME}" PROPERTIES OUTPUT_NAME "program.elf")
target_link_libraries("${PROJECT_NAME}" "nrf24" "printf")

set(c_compile_definitions "STM32L011xx")
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	list(APPEND c_compile_definitions "DEBUG" "_DEBUG")
else()
	list(APPEND c_compile_definitions "NDEBUG")
endif()

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

set(linker_script "${CMAKE_CURRENT_LIST_DIR}/../../../Libraries/Software/stm32l011f4.ld")
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

target_compile_options("${PROJECT_NAME}" PUBLIC "$<$<COMPILE_LANGUAGE:C>:${c_compile_options}>")
target_compile_options("${PROJECT_NAME}" PUBLIC "$<$<COMPILE_LANGUAGE:ASM>:${asm_compile_options}>")
target_compile_definitions("${PROJECT_NAME}" PUBLIC "$<$<COMPILE_LANGUAGE:C>:${c_compile_definitions}>")
set_target_properties("${PROJECT_NAME}" PROPERTIES LINK_FLAGS "${link_flags}")
