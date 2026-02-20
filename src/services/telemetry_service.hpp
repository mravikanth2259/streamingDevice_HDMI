/**
 * @file telemetry_service.hpp
 * @brief Telemetry/Logging Service
 */

#pragma once

#include <streaming_device/types.hpp>
#include <memory>
#include <string>

namespace streaming::services {

class ITelemetryService {
public:
    virtual ~ITelemetryService() = default;

    virtual device::Result initialize() = 0;
    virtual void shutdown() = 0;

    virtual void logEvent(const std::string& event_type, const std::string& payload) = 0;
    virtual void logError(const std::string& component, const std::string& message) = 0;
};

std::unique_ptr<ITelemetryService> createTelemetryService();

} // namespace streaming::services
