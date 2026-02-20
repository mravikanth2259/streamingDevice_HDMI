#pragma once

#include "../../hal/wifi_hal.hpp"

namespace streaming::drivers::mock {

class MockWifiDriver : public hal::IWifiHal {
public:
    device::Result initialize() override;
    device::Result shutdown() override;
    device::Result scan(std::vector<hal::WifiScanResult>& results) override;
    device::Result connect(const std::string& ssid, const std::string& password) override;
    device::Result disconnect() override;
    device::WifiState getState() const override;
    std::string getCurrentSsid() const override;
    void setStateCallback(hal::WifiStateCallback callback) override;
    std::string getLocalIp() const override;
    device::Result resolveHost(const std::string& hostname, std::string& ip_out) override;

private:
    device::WifiState state_{device::WifiState::DISCONNECTED};
    std::string ssid_;
    std::string local_ip_;
    hal::WifiStateCallback state_cb_;
};

} // namespace streaming::drivers::mock
