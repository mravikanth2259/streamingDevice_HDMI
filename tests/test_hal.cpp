/**
 * @file test_hal.cpp
 * @brief Unit tests for HAL layer (display, input, storage)
 */

#include "hal/display_hal.hpp"
#include "hal/input_hal.hpp"
#include "hal/storage_hal.hpp"
#include "drivers/mock/mock_input_driver.hpp"
#include <cassert>
#include <iostream>

#define ASSERT(cond) do { if (!(cond)) { std::cerr << "FAIL: " << #cond << "\n"; return 1; } } while(0)
#define TEST(name) std::cout << "Test: " << (name) << " ... "; std::cout.flush()

int main() {
    // Display HAL
    TEST("Display init");
    auto display = streaming::hal::createDisplayHal();
    ASSERT(display->initialize() == streaming::device::Result::OK);
    ASSERT(display->isConnected());
    ASSERT(display->getDisplayMode().width == 1920);
    ASSERT(display->getDisplayMode().height == 1080);
    ASSERT(display->clear() == streaming::device::Result::OK);
    auto modes = display->getSupportedModes();
    ASSERT(modes.size() >= 2);
    std::cout << "OK\n";

    // Input HAL
    TEST("Input init and inject");
    auto input = streaming::hal::createInputHal();
    ASSERT(input->initialize() == streaming::device::Result::OK);
    auto* mock_input = dynamic_cast<streaming::drivers::mock::MockInputDriver*>(input.get());
    ASSERT(mock_input != nullptr);
    mock_input->injectKey(streaming::device::RemoteKey::UP, streaming::hal::InputEventType::KEY_DOWN);
    ASSERT(input->getLastKey() == streaming::device::RemoteKey::UP);
    std::cout << "OK\n";

    // Storage HAL
    TEST("Storage read/write");
    auto storage = streaming::hal::createStorageHal();
    ASSERT(storage->initialize() == streaming::device::Result::OK);
    ASSERT(storage->write("test.key", "test.value") == streaming::device::Result::OK);
    std::string val;
    ASSERT(storage->read("test.key", val) == streaming::device::Result::OK);
    ASSERT(val == "test.value");
    ASSERT(storage->exists("test.key"));
    ASSERT(storage->remove("test.key") == streaming::device::Result::OK);
    ASSERT(!storage->exists("test.key"));
    std::cout << "OK\n";

    std::cout << "\nAll HAL tests passed.\n";
    return 0;
}
