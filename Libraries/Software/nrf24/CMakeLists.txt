project("nrf24")

file(GLOB_RECURSE sources "./*.[c|h]")

add_library("${PROJECT_NAME}" ${sources})
