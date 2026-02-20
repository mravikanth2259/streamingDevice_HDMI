/**
 * @file test_bluetooth_control.cpp
 * @brief Unit tests for Bluetooth control protocol
 */

#include "services/bluetooth_control_service.hpp"
#include <cassert>
#include <iostream>

#define ASSERT(cond) do { if (!(cond)) { std::cerr << "FAIL: " << #cond << "\n"; return 1; } } while(0)
#define TEST(name) std::cout << "Test: " << (name) << " ... "; std::cout.flush()

int main() {
    TEST("Bluetooth Control Service - init and command callback");
    auto bt_ctrl = streaming::services::createBluetoothControlService();
    ASSERT(bt_ctrl->initialize() == streaming::device::Result::OK);

    streaming::services::ControlCommand last_cmd = streaming::services::ControlCommand::UNKNOWN;
    bt_ctrl->setCommandCallback([&](streaming::services::ControlCommand cmd) {
        last_cmd = cmd;
    });

    // Simulate GATT write would require MockBluetoothDriver - for now just verify init
    ASSERT(bt_ctrl->sendStatus({0x01, 0x02}) == streaming::device::Result::OK);
    std::cout << "OK\n";

    std::cout << "\nBluetooth control tests passed.\n";
    return 0;
}
