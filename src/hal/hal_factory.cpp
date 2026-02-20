/**
 * @file hal_factory.cpp
 * @brief HAL factory - returns platform-specific or mock implementations
 *
 * Build with -DUSE_MOCK_HAL=ON for unit testing (mock drivers).
 * Otherwise uses platform drivers (when implemented).
 */

#include "display_hal.hpp"
#include "input_hal.hpp"
#include "wifi_hal.hpp"
#include "bluetooth_hal.hpp"
#include "hdmi_cec_hal.hpp"
#include "storage_hal.hpp"
#include "power_hal.hpp"
#include "audio_hal.hpp"
#include "../drivers/mock/mock_display_driver.hpp"
#include "../drivers/mock/mock_input_driver.hpp"
#include "../drivers/mock/mock_wifi_driver.hpp"
#include "../drivers/mock/mock_bluetooth_driver.hpp"
#include "../drivers/mock/mock_hdmi_cec_driver.hpp"
#include "../drivers/mock/mock_storage_driver.hpp"
#include "../drivers/mock/mock_power_driver.hpp"
#include "../drivers/mock/mock_audio_driver.hpp"

namespace streaming::hal {

// Uses mock drivers. Platform-specific builds can #ifdef to use real drivers.
using DisplayImpl = streaming::drivers::mock::MockDisplayDriver;
using InputImpl = streaming::drivers::mock::MockInputDriver;
using WifiImpl = streaming::drivers::mock::MockWifiDriver;
using BluetoothImpl = streaming::drivers::mock::MockBluetoothDriver;
using HdmiCecImpl = streaming::drivers::mock::MockHdmiCecDriver;
using StorageImpl = streaming::drivers::mock::MockStorageDriver;
using PowerImpl = streaming::drivers::mock::MockPowerDriver;
using AudioImpl = streaming::drivers::mock::MockAudioDriver;

std::unique_ptr<IDisplayHal> createDisplayHal() {
    return std::make_unique<DisplayImpl>();
}

std::unique_ptr<IInputHal> createInputHal() {
    return std::make_unique<InputImpl>();
}

std::unique_ptr<IWifiHal> createWifiHal() {
    return std::make_unique<WifiImpl>();
}

std::unique_ptr<IBluetoothHal> createBluetoothHal() {
    return std::make_unique<BluetoothImpl>();
}

std::unique_ptr<IHdmiCecHal> createHdmiCecHal() {
    return std::make_unique<HdmiCecImpl>();
}

std::unique_ptr<IStorageHal> createStorageHal() {
    return std::make_unique<StorageImpl>();
}

std::unique_ptr<IPowerHal> createPowerHal() {
    return std::make_unique<PowerImpl>();
}

std::unique_ptr<IAudioHal> createAudioHal() {
    return std::make_unique<AudioImpl>();
}

} // namespace streaming::hal
