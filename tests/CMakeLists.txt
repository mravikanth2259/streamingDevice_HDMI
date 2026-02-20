cmake_minimum_required(VERSION 3.14)

# Combined test runner
add_executable(streaming_device_tests
    test_runner.cpp
)

target_include_directories(streaming_device_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(streaming_device_tests PRIVATE streaming_device_lib)

target_compile_definitions(streaming_device_tests PRIVATE USE_MOCK_HAL=1)

add_test(NAME StreamingDeviceTests COMMAND streaming_device_tests)
