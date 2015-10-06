set(CXX_STANDARD_REQUIRED ON)

macro(gen_test TESTNAME SOURCE_LIST)
    # message("name: ${TESTNAME}, sources: ${SOURCE_LIST}")
    add_executable(${TESTNAME} ${SOURCE_LIST})
    target_link_libraries(${TESTNAME} ${PROJECT_NAME})
    add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
    set_property(TARGET ${TESTNAME} PROPERTY CXX_STANDARD 11)
endmacro()

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/extlib/cppmath/include
    ${PROJECT_SOURCE_DIR}/extlib/jsoncpp/include
)

gen_test(log log.cpp)
gen_test(factory factory.cpp)
