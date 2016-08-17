set (MODULE_NAME geometric_tools)

# Get all compilation units.
if(__LINUX__ OR __MAC_OS_X__)
	file (GLOB ${MODULE_NAME}_SRCS *Logger.cpp)
	# string (REGEX REPLACE "[^;]*GteDX[^;]*;" "" ${MODULE_NAME}_SRCS "${${MODULE_NAME}_SRCS}")
	# string (REGEX REPLACE "[^;]*GteComputeShader[^;]*;" "" ${MODULE_NAME}_SRCS "${${MODULE_NAME}_SRCS}")
	# string (REGEX REPLACE "[^;]*GteConstantColorEffect[^;]*;" "" ${MODULE_NAME}_SRCS "${${MODULE_NAME}_SRCS}")
	# string (REGEX REPLACE "[^;]*GteEnvironment[^;]*;" "" ${MODULE_NAME}_SRCS "${${MODULE_NAME}_SRCS}")
	# string (REGEX REPLACE "[^;]*GteFluid[^;]*;" "" ${MODULE_NAME}_SRCS "${${MODULE_NAME}_SRCS}")
else()
	file (GLOB ${MODULE_NAME}_SRCS *.cpp)
endif()

# Create the library.
add_library (${MODULE_NAME} ${${MODULE_NAME}_SRCS})

install (TARGETS ${MODULE_NAME} 
		ARCHIVE DESTINATION lib 
		LIBRARY DESTINATION lib)