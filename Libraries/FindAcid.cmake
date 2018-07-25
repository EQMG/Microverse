set(ACID_INCLUDES "${PROJECT_SOURCE_DIR}/Libraries/Acid/Sources")
set(ACID_BUILD_TESTING OFF CACHE INTERNAL "Build the Acid test programs")
set(ACID_INSTALL OFF CACHE INTERNAL "Generate installation target")
set(ACID_SETUP_COMPILER OFF CACHE INTERNAL "If Acid will set it's own outputs")
set(ACID_SETUP_OUTPUT OFF CACHE INTERNAL "If Acid will set it's own outputs")
add_subdirectory(${PROJECT_SOURCE_DIR}/Libraries/Acid)
set(ACID_LIBRARY "Acid")

set(LIBRARIES_INCLUDES ${LIBRARIES_INCLUDES} ${ACID_INCLUDES})
set(LIBRARIES_LINKS ${LIBRARIES_LINKS} "${ACID_LIBRARY}")

if(NOT ACID_LIBRARY)
	message(FATAL_ERROR "Acid library not found!")
endif()
