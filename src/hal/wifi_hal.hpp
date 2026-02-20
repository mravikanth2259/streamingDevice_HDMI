/**
 * @file wifi_hal.hpp
 * @brief Hardware Abstraction Layer for Wi-Fi connectivity
 * @copyright 2025 Streaming Device Project
 * 
 * Abstracts Wi-Fi driver, network stack, TLS/DTLS integration.
 */

#pragma once

#include <streaming_device/types.hpp>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace streaming::hal {

/** Wi-Fi scan result */
struct WifiScanResult {
    std::string ssid;
    std::string bssid;
    int32_t rssi{0};
    uint8_t security{0};  // 0=open, 1=WEP, 2=WPA, 3=WPA2
};

/** Connection callback */
using WifiStateCallback = std::function<void(device::WifiState)>;

/**
 * @brief Wi-Fi HAL Interface
 * 
 * Abstracts Wi-Fi client for streaming content. Supports scan, connect,
 * TLS for secure streaming pipelines.
 */
class IWifiHal {
public:
    virtual ~IWifiHal() = default;

    /** Initialize Wi-Fi subsystem */
    virtual device::Result initialize() = 0;

    /** Shutdown Wi-Fi */
    virtual device::Result shutdown() = 0;

    /** Scan for available networks */
    virtual device::Result scan(std::vector<WifiScanResult>& results) = 0;

    /** Connect to network with SSID and password */
    virtual device::Result connect(const std::string& ssid,
                                   const std::string& password) = 0;

    /** Disconnect from current network */
    virtual device::Result disconnect() = 0;

    /** Get current connection state */
    virtual device::WifiState getState() const = 0;

    /** Get current SSID if connected */
    virtual std::string getCurrentSsid() const = 0;

    /** Register state change callback */
    virtual void setStateCallback(WifiStateCallback callback) = 0;

    /** Get local IP address */
    virtual std::string getLocalIp() const = 0;

    /** Perform DNS resolution */
    virtual device::Result resolveHost(const std::string& hostname,
                                       std::string& ip_out) = 0;
};

/** Factory for creating platform-specific Wi-Fi HAL */
std::unique_ptr<IWifiHal> createWifiHal();

} // namespace streaming::hal
