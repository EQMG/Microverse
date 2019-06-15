find_path(ACID_INCLUDE_DIR 
        NAMES Acid.h
        ${ACID_FIND_ARGS}
        PATH_SUFFIXES 
                include/Acid 
                include 
                Acid
        )
find_library(ACID_LIBRARY
        NAMES acid
        ${ACID_FIND_ARGS}
        PATH_SUFFIXES 
                lib
        )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Acid
		REQUIRED_VARS ACID_LIBRARY ACID_INCLUDE_DIR
        )
        
if(Acid_FOUND)
    if(NOT TARGET Acid::Acid)
        add_library(Acid::Acid UNKNOWN IMPORTED)
        set_target_properties(Acid::Acid PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${ACID_INCLUDE_DIR}")
        set_target_properties(Acid::Acid PROPERTIES
                IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
                IMPORTED_LOCATION "${ACID_LIBRARY}"
                )
    endif()
endif()
