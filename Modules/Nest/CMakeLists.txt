# Created by Teivaz
# 16.02.2020

cmake_minimum_required(VERSION 3.12)

set(libraries_dir "${CMAKE_CURRENT_LIST_DIR}/../../Libraries")
set(CMAKE_TOOLCHAIN_FILE "${libraries_dir}/Software/toolchain.cmake")

project("Nest")

set(CMAKE_C_STANDARD "11")

set(linker_script "${libraries_dir}/Software/stm32wb55rg.ld")
set(device_name "STM32WB55xx")

add_compile_options(
	"-mcpu=cortex-m4"
	"-mthumb"
	"-mfloat-abi=hard"
	"-mfpu=fpv4-sp-d16"
	"--specs=nano.specs"
	"-ffunction-sections"
	"-fdata-sections"
	"-Wa,-adhlns=program.lst"
	"-Wall"
	"-fstack-usage"
	"-O0"
	"-g3"
	"-c"
)

set(compile_definitions "${device_name}")
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	list(APPEND compile_definitions "DEBUG" "_DEBUG")
else()
	list(APPEND compile_definitions "NDEBUG")
endif()
add_compile_definitions("${compile_definitions}")

file(GLOB_RECURSE sources "src/*.[c|h]")
add_executable("${PROJECT_NAME}" "${sources}")
set_target_properties("${PROJECT_NAME}" PROPERTIES OUTPUT_NAME "program.elf")
set_target_properties("${PROJECT_NAME}" PROPERTIES LINK_FLAGS
	"-mcpu=cortex-m4 \
	-T\"${linker_script}\" \
	-Wl,-Map=\"${PROJECT_NAME}.map\" \
	-Wl,--gc-sections \
	-static \
	--specs=nano.specs \
	-mfloat-abi=hard \
	-mfpu=fpv4-sp-d16 \
	-mthumb \
	-Wl,--start-group -lc -lm -Wl,--end-group"
)
