

add_subdirectory(serialize)
add_subdirectory(convert_dataset)

add_library(homework_5 homework_5.h)
target_link_libraries(homework_5 serialize convert_dataset)
target_include_directories(homework_5 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})