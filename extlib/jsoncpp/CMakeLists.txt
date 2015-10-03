set(CXX_STANDARD_REQUIRED ON)

include_directories(include)
add_library(jsoncpp src/json/jsoncpp.cpp)
set_property(TARGET jsoncpp  PROPERTY CXX_STANDARD 11)
