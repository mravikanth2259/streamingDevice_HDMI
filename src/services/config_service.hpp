/**
 * @file config_service.hpp
 * @brief Configuration Service - centralized settings with secure persistence
 */

#pragma once

#include <streaming_device/types.hpp>
#include <memory>
#include <string>

namespace streaming::services {

/**
 * @brief Config Service Interface
 */
class IConfigService {
public:
    virtual ~IConfigService() = default;

    virtual device::Result initialize() = 0;
    virtual void shutdown() = 0;

    virtual device::Result getString(const std::string& key, std::string& value) = 0;
    virtual device::Result setString(const std::string& key, const std::string& value) = 0;

    virtual device::Result getInt(const std::string& key, int32_t& value) = 0;
    virtual device::Result setInt(const std::string& key, int32_t value) = 0;

    virtual device::Result getBool(const std::string& key, bool& value) = 0;
    virtual device::Result setBool(const std::string& key, bool value) = 0;

    virtual device::Result save() = 0;
};

std::unique_ptr<IConfigService> createConfigService();

} // namespace streaming::services
