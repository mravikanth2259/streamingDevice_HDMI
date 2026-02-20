/**
 * @file config_service.cpp
 * @brief Config Service - uses Storage HAL
 */

#include "config_service.hpp"
#include "../hal/storage_hal.hpp"
#include <sstream>
#include <stdexcept>

namespace streaming::services {

class ConfigServiceImpl : public IConfigService {
public:
    ConfigServiceImpl() : storage_(hal::createStorageHal()) {}

    device::Result initialize() override {
        return storage_->initialize();
    }

    void shutdown() override {
        storage_->shutdown();
    }

    device::Result getString(const std::string& key, std::string& value) override {
        return storage_->read("config." + key, value);
    }

    device::Result setString(const std::string& key, const std::string& value) override {
        return storage_->write("config." + key, value);
    }

    device::Result getInt(const std::string& key, int32_t& value) override {
        std::string s;
        auto r = storage_->read("config." + key, s);
        if (r != device::Result::OK) return r;
        try {
            value = std::stoi(s);
        } catch (const std::invalid_argument&) {
            return device::Result::ERROR_INVALID_PARAM;
        } catch (const std::out_of_range&) {
            return device::Result::ERROR_INVALID_PARAM;
        }
        return device::Result::OK;
    }

    device::Result setInt(const std::string& key, int32_t value) override {
        return storage_->write("config." + key, std::to_string(value));
    }

    device::Result getBool(const std::string& key, bool& value) override {
        std::string s;
        auto r = storage_->read("config." + key, s);
        if (r != device::Result::OK) return r;
        value = (s == "1" || s == "true");
        return device::Result::OK;
    }

    device::Result setBool(const std::string& key, bool value) override {
        return storage_->write("config." + key, value ? "1" : "0");
    }

    device::Result save() override {
        return device::Result::OK;  // Storage HAL persists on write
    }

private:
    std::unique_ptr<hal::IStorageHal> storage_;
};

std::unique_ptr<IConfigService> createConfigService() {
    return std::make_unique<ConfigServiceImpl>();
}

} // namespace streaming::services
