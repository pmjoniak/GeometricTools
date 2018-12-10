set (MODULE_NAME geometric_tools)

# Get all compilation units.
if(UNIX AND NOT APPLE)	
	add_definitions(-DGTE_DISABLE_PCH)
	file (GLOB ${MODULE_NAME}_SRCS Mathematics/*.cpp)
	file (GLOB ${MODULE_NAME}_SRCS LowLevel/*.cpp)
elseif(APPLE)
	add_definitions(-DGTE_DISABLE_PCH)
	file (GLOB ${MODULE_NAME}_SRCS Mathematics/*.cpp)
	file (GLOB ${MODULE_NAME}_SRCS LowLevel/*.cpp)
else()
	file (GLOB ${MODULE_NAME}_SRCS *.cpp)
endif()

# Create the library.
add_library (${MODULE_NAME} STATIC ${${MODULE_NAME}_SRCS})

install (TARGETS ${MODULE_NAME}
          RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
          LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
          ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )