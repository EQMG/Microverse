set(FLOUNDER_INCLUDES "${PROJECT_SOURCE_DIR}/Libraries/Flounder/Sources/FlounderEngine")
set(FLOUNDER_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the Flounder example programs")
set(FLOUNDER_BUILD_TESTS OFF CACHE INTERNAL "Build the Flounder test programs")
set(FLOUNDER_INSTALL OFF CACHE INTERNAL "Generate installation target")
set(FLOUNDER_SET_OUTPUT OFF CACHE INTERNAL "If Flounder will set it's own outputs")
add_subdirectory(${PROJECT_SOURCE_DIR}/Libraries/Flounder)
set(FLOUNDER_LIBRARY "FlounderEngine")

set(LIBRARIES_INCLUDES ${LIBRARIES_INCLUDES} ${FLOUNDER_INCLUDES})
set(LIBRARIES_LINKS ${LIBRARIES_LINKS} "${FLOUNDER_LIBRARY}")

if(NOT FLOUNDER_LIBRARY)
	message(FATAL_ERROR "Flounder library not found!")
endif()
