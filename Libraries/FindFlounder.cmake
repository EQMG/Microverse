set(FLOUNDER_INCLUDES "${PROJECT_SOURCE_DIR}/Libraries/Flounder/Sources")
set(FLOUNDER_BUILD_TESTING OFF CACHE INTERNAL "Build the Flounder test programs")
set(FLOUNDER_INSTALL OFF CACHE INTERNAL "Generate installation target")
set(FLOUNDER_SETUP_COMPILER OFF CACHE INTERNAL "If Flounder will set it's own outputs")
set(FLOUNDER_SETUP_OUTPUT OFF CACHE INTERNAL "If Flounder will set it's own outputs")
add_subdirectory(${PROJECT_SOURCE_DIR}/Libraries/Flounder)
set(FLOUNDER_LIBRARY "Flounder")

set(LIBRARIES_INCLUDES ${LIBRARIES_INCLUDES} ${FLOUNDER_INCLUDES})
set(LIBRARIES_LINKS ${LIBRARIES_LINKS} "${FLOUNDER_LIBRARY}")

if(NOT FLOUNDER_LIBRARY)
	message(FATAL_ERROR "Flounder library not found!")
endif()
