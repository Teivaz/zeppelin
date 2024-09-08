project("STM32WBxx_HAL")

file(GLOB_RECURSE headers "Inc/*.h")
file(GLOB_RECURSE sources "Src/*.c")

# To switch between HAL/LL use define this macro:
# USE_FULL_LL_DRIVER

add_library("${PROJECT_NAME}" ${sources} ${headers})

target_link_libraries("${PROJECT_NAME}" "CMSIS")

target_include_directories("${PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_LIST_DIR}/Inc")
