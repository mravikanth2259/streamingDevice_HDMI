/**
 * @file bluetooth_hal.hpp
 * @brief Hardware Abstraction Layer for Bluetooth
 * @copyright 2025 Streaming Device Project
 * 
 * Abstracts radio management, A2DP (audio sink), HFP, GATT (control channel).
 */

#pragma once

#include <streaming_device/types.hpp>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace streaming::hal {

/** Bluetooth device info */
struct BluetoothDeviceInfo {
    std::string address;
    std::string name;
    bool paired{false};
};

/** GATT characteristic UUID (128-bit as string) */
using GattUuid = std::string;

/** Data received callback */
using GattDataCallback = std::function<void(const std::vector<uint8_t>&)>;

/**
 * @brief Bluetooth HAL Interface
 * 
 * - A2DP sink for audio to external speakers
 * - GATT service for mobile app control channel
 * - Power management for Bluetooth radio
 */
class IBluetoothHal {
public:
    virtual ~IBluetoothHal() = default;

    /** Initialize Bluetooth radio */
    virtual device::Result initialize() = 0;

    /** Shutdown Bluetooth */
    virtual device::Result shutdown() = 0;

    /** Enable discoverable mode for pairing */
    virtual device::Result setDiscoverable(bool discoverable) = 0;

    /** Get current state */
    virtual device::BluetoothState getState() const = 0;

    /** Get list of paired devices */
    virtual device::Result getPairedDevices(std::vector<BluetoothDeviceInfo>& out) = 0;

    /** Unpair device by address */
    virtual device::Result unpair(const std::string& address) = 0;

    // --- GATT Control Channel (for mobile app) ---

    /** Register GATT service for control */
    virtual device::Result registerControlService(
        const GattUuid& service_uuid,
        const std::vector<GattUuid>& characteristic_uuids) = 0;

    /** Write to GATT characteristic (notify mobile app) */
    virtual device::Result writeCharacteristic(
        const GattUuid& char_uuid,
        const std::vector<uint8_t>& data) = 0;

    /** Set callback for incoming GATT writes from mobile */
    virtual void setGattWriteCallback(GattUuid char_uuid, GattDataCallback cb) = 0;

    // --- A2DP Audio Sink ---

    /** Connect as A2DP sink to audio source */
    virtual device::Result connectA2dpSink(const std::string& device_address) = 0;

    /** Disconnect A2DP */
    virtual device::Result disconnectA2dp() = 0;

    /** Check if A2DP is connected */
    virtual bool isA2dpConnected() const = 0;
};

/** Factory for creating platform-specific Bluetooth HAL */
std::unique_ptr<IBluetoothHal> createBluetoothHal();

} // namespace streaming::hal
