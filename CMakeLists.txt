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


# Testing

# Build tests if this is the root level project
set( BUILD_TESTING ON CACHE BOOL "Build tests" FORCE )

# set( BUILD_TESTING_DEFAULT ${PROJECT_IS_TOP_LEVEL} )
# set( BUILD_TESTING_DEFAULT ON )
# option( BUILD_TESTING "Build tests" ${PROJECT_IS_TOP_LEVEL} )

if( BUILD_TESTING )
	include( CTest )

	find_package( GTest REQUIRED )

	# define the test sources
	file( GLOB_RECURSE TEST_SOURCES
		CONFIGURE_DEPENDS
			"${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/test/**/*.cpp"
	)

	add_executable( ${PROJECT_NAME}_tests
			${TEST_SOURCES}
	)

	target_link_libraries( ${PROJECT_NAME}_tests
		PRIVATE
			${PROJECT_NAME}
	  		GTest::gtest_main
	)

	include( GoogleTest )

	gtest_discover_tests( ${PROJECT_NAME}_tests
		TEST_PREFIX
			"${PROJECT_NAME}::"
	)
endif()
