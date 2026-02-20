#include "mock_bluetooth_driver.hpp"

namespace streaming::drivers::mock {

device::Result MockBluetoothDriver::initialize() { return device::Result::OK; }
device::Result MockBluetoothDriver::shutdown() { return device::Result::OK; }

device::Result MockBluetoothDriver::setDiscoverable(bool) { return device::Result::OK; }
device::BluetoothState MockBluetoothDriver::getState() const { return state_; }

device::Result MockBluetoothDriver::getPairedDevices(std::vector<hal::BluetoothDeviceInfo>& out) {
    out = {{"AA:BB:CC:DD:EE:FF", "Test Phone", true}};
    return device::Result::OK;
}

device::Result MockBluetoothDriver::unpair(const std::string&) { return device::Result::OK; }

device::Result MockBluetoothDriver::registerControlService(const hal::GattUuid&,
    const std::vector<hal::GattUuid>&) {
    return device::Result::OK;
}

device::Result MockBluetoothDriver::writeCharacteristic(const hal::GattUuid&,
    const std::vector<uint8_t>&) {
    return device::Result::OK;
}

void MockBluetoothDriver::setGattWriteCallback(hal::GattUuid char_uuid, hal::GattDataCallback cb) {
    gatt_callbacks_[char_uuid] = std::move(cb);
}

device::Result MockBluetoothDriver::connectA2dpSink(const std::string&) {
    a2dp_connected_ = true;
    return device::Result::OK;
}

device::Result MockBluetoothDriver::disconnectA2dp() {
    a2dp_connected_ = false;
    return device::Result::OK;
}

bool MockBluetoothDriver::isA2dpConnected() const { return a2dp_connected_; }

void MockBluetoothDriver::simulateGattWrite(const hal::GattUuid& uuid, const std::vector<uint8_t>& data) {
    auto it = gatt_callbacks_.find(uuid);
    if (it != gatt_callbacks_.end()) it->second(data);
}

} // namespace streaming::drivers::mock
