
cmake_minimum_required(VERSION 2.6)

project(DataStructureAndAlgo)

add_compile_options(-g)

set(CMAKE_EXE_LINKER_FLAGS -g)

file(GLOB SOURCES *.c)
foreach (sourcefile ${SOURCES})
	get_filename_component(name ${sourcefile} NAME)
	string(REGEX REPLACE "t[0-9]+_[0-9]+\\.c" "" name ${name})
	MESSAGE(STATUS "name = " ${name})
	if (name STRGREATER "")
		MESSAGE(STATUS "t1 name = " ${name})
		string(REGEX REPLACE ".*_test.c" "" name ${name})
		MESSAGE(STATUS "t2 name = " ${name})
		if (name STRGREATER "")
			MESSAGE(STATUS "t3 name = " ${name})
			set(LIB_SOURCES ${LIB_SOURCES} ${name})
			MESSAGE(STATUS "lib_sources = " ${LIB_SOURCES})
		endif()
	endif()
endforeach()

add_library(util STATIC ${LIB_SOURCES})

set(COMMON_LIBS util pthread rt)

file(GLOB TEST_SOURCES *_test.c)

foreach (testsourcefile ${TEST_SOURCES})
	get_filename_component(testname ${testsourcefile} NAME)
	string(REPLACE ".c" "" testname ${testname})
	add_executable(${testname} ${testsourcefile})
	target_link_libraries(${testname} ${COMMON_LIBS})
endforeach()

file(GLOB SOURCES t*_*.c)
MESSAGE(STATUS "SOURCES = " ${SOURCES})
string(REGEX MATCHALL "t([0-9]+)_([0-9]+)\\.c" SOURCES ${SOURCES})
MESSAGE(STATUS "SOURCES = " ${SOURCES})

foreach (sourcefile ${SOURCES})
	get_filename_component(target ${sourcefile} NAME)
	string(REPLACE ".c" "" target ${target})
	add_executable(${target} ${sourcefile})
	target_link_libraries(${target} ${COMMON_LIBS})
endforeach()
