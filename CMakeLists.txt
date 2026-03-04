cmake_minimum_required( VERSION 3.21 )

project( common
  VERSION 1.0
  LANGUAGES CXX
)

set( CMAKE_EXPORT_COMPILE_COMMANDS on )

set( CMAKE_CXX_STANDARD_REQUIRED on )
set( CMAKE_CXX_STANDARD 23 )

if( NOT TARGET meta )
	add_subdirectory( ${PROJECT_SOURCE_DIR}/external/meta )
endif()

# Compiling

# define the library sources
file( GLOB_RECURSE SOURCES
	CONFIGURE_DEPENDS
		"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp"
)

add_library( ${PROJECT_NAME}
	STATIC
		${SOURCES}
)

target_include_directories( ${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		
)

get_target_property( INCL_DIRS ${PROJECT_NAME} INTERFACE_INCLUDE_DIRECTORIES )
message( STATUS "Includes for ${PROJECT_NAME}: ${INCL_DIRS}" )


# Linking

target_link_libraries( ${PROJECT_NAME}
	PUBLIC
		meta
)

