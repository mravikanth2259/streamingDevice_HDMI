/**
 * @file update_service.hpp
 * @brief OTA Update Service
 */

#pragma once

#include <streaming_device/types.hpp>
#include <functional>
#include <memory>
#include <string>

namespace streaming::services {

using UpdateProgressCallback = std::function<void(uint8_t percent, const std::string& status)>;

class IUpdateService {
public:
    virtual ~IUpdateService() = default;

    virtual device::Result initialize() = 0;
    virtual void shutdown() = 0;

    virtual device::Result checkForUpdates() = 0;
    virtual device::Result downloadAndApply(const std::string& update_url,
                                           UpdateProgressCallback progress) = 0;
};

std::unique_ptr<IUpdateService> createUpdateService();

} // namespace streaming::services
