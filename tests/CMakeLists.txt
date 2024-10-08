include(/home/stefania/Downloads/vcpkg/scripts/buildsystems/vcpkg.cmake)
find_package(Catch2 REQUIRED)
include(CTest)
include(Catch)

find_package(OpenCV 4 REQUIRED)
add_library(sifts_internal SHARED sifts_internal.cpp sifts_internal.hpp)
target_link_libraries(sifts_internal ${OpenCV_LIBS})

set(TEST_BINARY ${PROJECT_NAME}_test)
add_executable(${TEST_BINARY}
               test_serialize.cpp
               test_convert_dataset.cpp
               test_main.cpp)

target_link_libraries(${TEST_BINARY}    
                    serialize                    
                    convert_dataset                        
                    homework_5
                      sifts_internal
                      Catch2::Catch2)

catch_discover_tests(${TEST_BINARY} WORKING_DIRECTORY
                     ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(sifts_internal PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
