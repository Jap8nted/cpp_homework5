add_library(serialize serialize.cpp serialize.hpp)
find_package(OpenCV 4 REQUIRED)

target_link_libraries(serialize ${OpenCV_LIBS} sifts_internal)
target_include_directories(serialize PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})