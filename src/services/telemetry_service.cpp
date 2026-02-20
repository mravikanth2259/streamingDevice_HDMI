/**
 * @file telemetry_service.cpp
 */

#include "telemetry_service.hpp"
#include "../common/logger.hpp"

namespace streaming::services {

class TelemetryServiceImpl : public ITelemetryService {
public:
    device::Result initialize() override { return device::Result::OK; }
    void shutdown() override {}

    void logEvent(const std::string& event_type, const std::string& payload) override {
        LOG_INFO("Telemetry", event_type, payload);
    }

    void logError(const std::string& component, const std::string& message) override {
        LOG_ERROR("Telemetry", component, message);
    }
};

std::unique_ptr<ITelemetryService> createTelemetryService() {
    return std::make_unique<TelemetryServiceImpl>();
}

} // namespace streaming::services
