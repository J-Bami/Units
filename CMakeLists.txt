
cmake_minimum_required (VERSION 3.18)

project(Units)

add_library(LibUnits STATIC 
	"dimension.h"
	"quantity.h"
	"quantities.h"
	"util.h"  
	"typelist.h" 
	"dimensions.h" 
	"conversions.h" 
	"unit.h"
	"units.h")

set_target_properties(LibUnits PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(LibUnits PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(TestUnits tests.cpp)
target_link_libraries(TestUnits INTERFACE LibUnits)