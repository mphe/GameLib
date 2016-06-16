set(CXX_STANDARD_REQUIRED ON)

macro(gen_test_full TESTNAME SOURCE)
    add_executable(${TESTNAME} ${SOURCE} ${ARGN})
    target_link_libraries(${TESTNAME} ${PROJECT_NAME})
    add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
    set_property(TARGET ${TESTNAME} PROPERTY CXX_STANDARD 11)
endmacro()

macro(gen_test TESTNAME SOURCE)
    add_executable(${TESTNAME} ${SOURCE} ${ARGN})
    add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
    set_property(TARGET ${TESTNAME} PROPERTY CXX_STANDARD 11)
endmacro()

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/extlib/cppmath/include
    ${PROJECT_SOURCE_DIR}/extlib/jsoncpp/include
)

gen_test(factorytest factory.cpp)
gen_test(logtest log.cpp)
gen_test(stablevector_test stablevector_test.cpp)
gen_test_full(statictilemap statictilemap.cpp)
gen_test_full(static_dataset_tilemap static_dataset_tilemap.cpp)
gen_test_full(entity_component entity_component.cpp)
gen_test_full(updatesystem_test ./updatesystem.cpp)
