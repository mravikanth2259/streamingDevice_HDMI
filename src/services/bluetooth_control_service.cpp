/**
 * @file bluetooth_control_service.cpp
 * @brief Bluetooth Control Service - GATT protocol handler
 */

#include "bluetooth_control_service.hpp"
#include "../hal/bluetooth_hal.hpp"
#include "../common/event_bus.hpp"
#include "../common/logger.hpp"

namespace streaming::services {

// GATT UUIDs for control service (example - customize per platform)
constexpr const char* CONTROL_SERVICE_UUID = "12345678-1234-5678-1234-56789abcdef0";
constexpr const char* COMMAND_CHAR_UUID   = "12345678-1234-5678-1234-56789abcdef1";
constexpr const char* STATUS_CHAR_UUID    = "12345678-1234-5678-1234-56789abcdef2";

static ControlCommand parseCommand(uint8_t byte) {
    switch (byte) {
        case 0x01: return ControlCommand::NAV_UP;
        case 0x02: return ControlCommand::NAV_DOWN;
        case 0x03: return ControlCommand::NAV_LEFT;
        case 0x04: return ControlCommand::NAV_RIGHT;
        case 0x05: return ControlCommand::SELECT;
        case 0x06: return ControlCommand::BACK;
        case 0x07: return ControlCommand::PLAY;
        case 0x08: return ControlCommand::PAUSE;
        case 0x09: return ControlCommand::VOLUME_UP;
        case 0x0A: return ControlCommand::VOLUME_DOWN;
        case 0x0B: return ControlCommand::POWER;
        default:   return ControlCommand::UNKNOWN;
    }
}

class BluetoothControlServiceImpl : public IBluetoothControlService {
public:
    device::Result initialize() override {
        auto bt = hal::createBluetoothHal();
        if (bt->initialize() != device::Result::OK) return device::Result::ERROR_GENERIC;

        bt->registerControlService(CONTROL_SERVICE_UUID, {COMMAND_CHAR_UUID, STATUS_CHAR_UUID});
        bt->setGattWriteCallback(COMMAND_CHAR_UUID, [this](const std::vector<uint8_t>& data) {
            if (!data.empty() && command_cb_) {
                command_cb_(parseCommand(data[0]));
            }
        });

        bt_ = std::move(bt);
        return device::Result::OK;
    }

    void shutdown() override {
        if (bt_) bt_->shutdown();
    }

    void setCommandCallback(ControlCommandCallback cb) override {
        command_cb_ = std::move(cb);
    }

    device::Result sendStatus(const std::vector<uint8_t>& data) override {
        if (bt_) return bt_->writeCharacteristic(STATUS_CHAR_UUID, data);
        return device::Result::ERROR_GENERIC;
    }

    bool isConnected() const override {
        return bt_ && bt_->getState() == device::BluetoothState::CONNECTED;
    }

private:
    std::unique_ptr<hal::IBluetoothHal> bt_;
    ControlCommandCallback command_cb_;
};

std::unique_ptr<IBluetoothControlService> createBluetoothControlService() {
    return std::make_unique<BluetoothControlServiceImpl>();
}

} // namespace streaming::services
