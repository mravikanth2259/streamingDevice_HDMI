#include "mock_wifi_driver.hpp"

namespace streaming::drivers::mock {

device::Result MockWifiDriver::initialize() { return device::Result::OK; }
device::Result MockWifiDriver::shutdown() { return device::Result::OK; }

device::Result MockWifiDriver::scan(std::vector<hal::WifiScanResult>& results) {
    results = {{"TestNetwork", "aa:bb:cc:dd:ee:ff", -55, 2}};
    return device::Result::OK;
}

device::Result MockWifiDriver::connect(const std::string& ssid, const std::string& /*password*/) {
    ssid_ = ssid;
    state_ = device::WifiState::CONNECTED;
    local_ip_ = "192.168.1.100";
    if (state_cb_) state_cb_(state_);
    return device::Result::OK;
}

device::Result MockWifiDriver::disconnect() {
    state_ = device::WifiState::DISCONNECTED;
    ssid_.clear();
    if (state_cb_) state_cb_(state_);
    return device::Result::OK;
}

device::WifiState MockWifiDriver::getState() const { return state_; }
std::string MockWifiDriver::getCurrentSsid() const { return ssid_; }

void MockWifiDriver::setStateCallback(hal::WifiStateCallback callback) {
    state_cb_ = std::move(callback);
}

std::string MockWifiDriver::getLocalIp() const { return local_ip_; }

device::Result MockWifiDriver::resolveHost(const std::string& hostname, std::string& ip_out) {
    if (hostname == "api.netflix.com") { ip_out = "52.1.2.3"; return device::Result::OK; }
    if (hostname == "api.amazon.com") { ip_out = "54.2.3.4"; return device::Result::OK; }
    ip_out = "127.0.0.1";
    return device::Result::OK;
}

} // namespace streaming::drivers::mock
