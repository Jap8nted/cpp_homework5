find_package(OpenCV 4 REQUIRED)
add_library(mat_serialization serialize.hpp convert_dataset.hpp homework_5.h)

target_link_libraries(mat_serialization ${OpenCV_LIBS})
set_target_properties(mat_serialization PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(mat_serialization PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})