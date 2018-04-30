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
# gen_test(tilemap_collider_test tilemap_collider.cpp)
gen_test(slotmap slotmap.cpp)
gen_test_full(statictilemap statictilemap.cpp)
gen_test_full(static_dataset_tilemap static_dataset_tilemap.cpp)
gen_test_full(resmgr resmgr.cpp)
gen_test_full(ecs ecs.cpp)
gen_test_full(entityfactory entityfactory.cpp)
gen_test_full(properties properties.cpp)
# gen_test_full(entity_component legacy/entity_component.cpp)
# gen_test_full(updatesystem_test legacy/updatesystem.cpp)
