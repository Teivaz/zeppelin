project("protocol")
file(GLOB_RECURSE sources "./*.[c|h]")
add_library("${PROJECT_NAME}" ${sources})
target_include_directories("${PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
