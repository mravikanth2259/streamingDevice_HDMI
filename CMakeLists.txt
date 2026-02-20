cmake_minimum_required(VERSION 3.14)
project(StreamingDevice VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Options
option(USE_MOCK_HAL "Use mock HAL drivers for testing" ON)
option(BUILD_TESTS "Build unit and integration tests" ON)

# Include paths
set(INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# HAL sources
set(HAL_SOURCES
    src/hal/hal_factory.cpp
    src/hal/media_hal_factory.cpp
)

# Mock driver sources
set(MOCK_DRIVER_SOURCES
    src/drivers/mock/mock_display_driver.cpp
    src/drivers/mock/mock_input_driver.cpp
    src/drivers/mock/mock_wifi_driver.cpp
    src/drivers/mock/mock_bluetooth_driver.cpp
    src/drivers/mock/mock_hdmi_cec_driver.cpp
    src/drivers/mock/mock_storage_driver.cpp
    src/drivers/mock/mock_power_driver.cpp
    src/drivers/mock/mock_audio_driver.cpp
    src/drivers/mock/mock_codec_decoder.cpp
    src/drivers/mock/mock_container_parser.cpp
    src/drivers/mock/mock_video_pipeline.cpp
)

# Common sources
set(COMMON_SOURCES
    src/common/event_bus.cpp
)

# Service sources
set(SERVICE_SOURCES
    src/services/app_launcher_service.cpp
    src/services/ui_service.cpp
    src/services/streaming_service.cpp
    src/services/hdmi_cec_service.cpp
    src/services/bluetooth_control_service.cpp
    src/services/config_service.cpp
    src/services/telemetry_service.cpp
    src/services/update_service.cpp
    src/services/codec_service.cpp
    src/services/container_service.cpp
    src/services/stream_pipeline_service.cpp
)

# Main executable
add_executable(streaming_device src/main.cpp)

target_include_directories(streaming_device PRIVATE ${INCLUDE_DIRS})

if(USE_MOCK_HAL)
    target_compile_definitions(streaming_device PRIVATE USE_MOCK_HAL=1)
endif()

# Streaming device library (for tests)
add_library(streaming_device_lib STATIC
    ${HAL_SOURCES}
    ${MOCK_DRIVER_SOURCES}
    ${COMMON_SOURCES}
    ${SERVICE_SOURCES}
)

target_include_directories(streaming_device_lib PUBLIC ${INCLUDE_DIRS})

if(USE_MOCK_HAL)
    target_compile_definitions(streaming_device_lib PUBLIC USE_MOCK_HAL=1)
endif()

# Link main against lib
target_link_libraries(streaming_device PRIVATE streaming_device_lib)

# Tests
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Install
install(TARGETS streaming_device DESTINATION bin)
