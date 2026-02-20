#pragma once

#include "../../hal/bluetooth_hal.hpp"
#include <map>

namespace streaming::drivers::mock {

class MockBluetoothDriver : public hal::IBluetoothHal {
public:
    device::Result initialize() override;
    device::Result shutdown() override;
    device::Result setDiscoverable(bool discoverable) override;
    device::BluetoothState getState() const override;
    device::Result getPairedDevices(std::vector<hal::BluetoothDeviceInfo>& out) override;
    device::Result unpair(const std::string& address) override;
    device::Result registerControlService(const hal::GattUuid& service_uuid,
        const std::vector<hal::GattUuid>& characteristic_uuids) override;
    device::Result writeCharacteristic(const hal::GattUuid& char_uuid,
        const std::vector<uint8_t>& data) override;
    void setGattWriteCallback(hal::GattUuid char_uuid, hal::GattDataCallback cb) override;
    device::Result connectA2dpSink(const std::string& device_address) override;
    device::Result disconnectA2dp() override;
    bool isA2dpConnected() const override;

    void simulateGattWrite(const hal::GattUuid& uuid, const std::vector<uint8_t>& data);

private:
    device::BluetoothState state_{device::BluetoothState::OFF};
    bool a2dp_connected_{false};
    std::map<hal::GattUuid, hal::GattDataCallback> gatt_callbacks_;
};

} // namespace streaming::drivers::mock
