# automatically get mod name from mod directory name
get_filename_component(MOD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/.. ABSOLUTE)
get_filename_component(MY_MOD_NAME ${MOD_DIR} NAME)
string(REPLACE " " "_" MY_MOD_NAME ${MY_MOD_NAME})
project(${MY_MOD_NAME} C CXX)

# add all source files in src/ directory
file(GLOB SRC *.cpp)
add_library(${MY_MOD_NAME} SHARED ${SRC})

# precomple headers and link to quark_engine
target_precompile_headers(${MY_MOD_NAME} PUBLIC ${QUARK_MODULE})
target_link_libraries(${MY_MOD_NAME} quark_engine)

FILE(STRINGS "../deps.txt" DEPS)

# precompile headers and link to all other dependencies
foreach(DEP ${DEPS})
	get_target_property(DIR ${DEP} SOURCE_DIR)
	target_precompile_headers(${MY_MOD_NAME} PUBLIC ${DIR}/module.hpp)
	message(${DIR}/module.hpp)
endforeach()

foreach(DEP ${DEPS})
	target_link_libraries(${MY_MOD_NAME} ${DEP})
endforeach()
