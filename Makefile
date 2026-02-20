# Makefile for streaming device (fallback when CMake unavailable)
CXX = g++
CXXFLAGS = -std=c++17 -I include -I src -Wall -Wextra

LIB_SRCS = \
	src/hal/hal_factory.cpp \
	src/hal/media_hal_factory.cpp \
	src/drivers/mock/mock_display_driver.cpp \
	src/drivers/mock/mock_input_driver.cpp \
	src/drivers/mock/mock_wifi_driver.cpp \
	src/drivers/mock/mock_bluetooth_driver.cpp \
	src/drivers/mock/mock_hdmi_cec_driver.cpp \
	src/drivers/mock/mock_storage_driver.cpp \
	src/drivers/mock/mock_power_driver.cpp \
	src/drivers/mock/mock_audio_driver.cpp \
	src/drivers/mock/mock_codec_decoder.cpp \
	src/drivers/mock/mock_container_parser.cpp \
	src/drivers/mock/mock_video_pipeline.cpp \
	src/common/event_bus.cpp \
	src/services/app_launcher_service.cpp \
	src/services/ui_service.cpp \
	src/services/streaming_service.cpp \
	src/services/hdmi_cec_service.cpp \
	src/services/bluetooth_control_service.cpp \
	src/services/config_service.cpp \
	src/services/telemetry_service.cpp \
	src/services/update_service.cpp \
	src/services/codec_service.cpp \
	src/services/container_service.cpp \
	src/services/stream_pipeline_service.cpp

.PHONY: all clean test run

all: build/streaming_device build/streaming_device_tests

build/streaming_device: src/main.cpp $(LIB_SRCS)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $^ -o $@

build/streaming_device_tests: tests/test_runner.cpp $(LIB_SRCS)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $^ -o $@

test: build/streaming_device_tests
	./build/streaming_device_tests

run: build/streaming_device
	./build/streaming_device

clean:
	rm -rf build
