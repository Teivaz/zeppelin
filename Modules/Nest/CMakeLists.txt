# Created by Teivaz
# 16.02.2020

cmake_minimum_required(VERSION 3.12)

set(libraries_dir "${CMAKE_CURRENT_LIST_DIR}/../../Libraries")
set(CMAKE_TOOLCHAIN_FILE "${libraries_dir}/Software/toolchain.cmake")

project("Nest")

set(CMAKE_C_STANDARD "11")

set(linker_script "${libraries_dir}/Software/stm32wb55rg-wFlash.ld")
set(device_name "STM32WB55xx")

add_compile_options(
	"-mcpu=cortex-m4"
	"-mthumb"
	"-mfloat-abi=hard"
	"-mfpu=fpv4-sp-d16"
	"--specs=nano.specs"
	"-Wa,-adhlns=program.lst"
	"-Wall"
	
	"-Wno-unused-function" # for development
	"-Wno-missing-braces" # this warning is wrong
	
	"-ffunction-sections"
	"-fdata-sections"
	"-fstack-usage"
	"-O0"
	"-g3"
	"-c"
)

set(compile_definitions "${device_name}" "USE_HAL_DRIVER")
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	list(APPEND compile_definitions "DEBUG" "_DEBUG")
else()
	list(APPEND compile_definitions "NDEBUG")
endif()
add_compile_definitions("${compile_definitions}")

add_subdirectory("${libraries_dir}/Software/STM32WBxx_HAL_Driver" "STM32WBxx_HAL")
add_subdirectory("${libraries_dir}/Software/CMSIS" "CMSIS")
add_subdirectory("${libraries_dir}/Software/protocol" "protocol")

file(GLOB_RECURSE sources "src/*.[c|h]")
add_executable("${PROJECT_NAME}" "${sources}")
set_target_properties("${PROJECT_NAME}" PROPERTIES OUTPUT_NAME "program.elf")
target_link_libraries("${PROJECT_NAME}" "STM32WBxx_HAL" "protocol")
target_include_directories("STM32WBxx_HAL" PUBLIC "${CMAKE_CURRENT_LIST_DIR}/config")

target_include_directories("${PROJECT_NAME}" PUBLIC
	"${CMAKE_CURRENT_LIST_DIR}/src/"
	"${CMAKE_CURRENT_LIST_DIR}/src/trash_from_st/src"
	"${CMAKE_CURRENT_LIST_DIR}/src/trash_from_st/inc"
	"${CMAKE_CURRENT_LIST_DIR}/src/trash_from_st/sequencer"
	"${CMAKE_CURRENT_LIST_DIR}/src/trash_from_st/wpan"
	"${CMAKE_CURRENT_LIST_DIR}/src/trash_from_st/wpan_middleware/"
	"${CMAKE_CURRENT_LIST_DIR}/src/trash_from_st/wpan_middleware/ble"
	"${CMAKE_CURRENT_LIST_DIR}/src/trash_from_st/wpan_middleware/ble/core"
	"${CMAKE_CURRENT_LIST_DIR}/src/trash_from_st/wpan_middleware/ble/core/template"
	"${CMAKE_CURRENT_LIST_DIR}/src/trash_from_st/wpan_middleware/interface"
	"${CMAKE_CURRENT_LIST_DIR}/src/trash_from_st/wpan_middleware/interface/shci"
	"${CMAKE_CURRENT_LIST_DIR}/src/trash_from_st/wpan_middleware/interface/tl"
	"${CMAKE_CURRENT_LIST_DIR}/src/trash_from_st/wpan_middleware/utilities"
	"${CMAKE_CURRENT_LIST_DIR}/src/trash_from_st/tiny_lpm"
)

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
