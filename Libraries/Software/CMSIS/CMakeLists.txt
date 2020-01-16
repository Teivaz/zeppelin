project("CMSIS")

file(GLOB_RECURSE sources "./*.[c|h]")

add_library("${PROJECT_NAME}" ${sources})

target_include_directories("${PROJECT_NAME}" PUBLIC
	"${CMAKE_CURRENT_LIST_DIR}/Include"
	"${CMAKE_CURRENT_LIST_DIR}/Device/St/STM32L0xx/Include"
)
