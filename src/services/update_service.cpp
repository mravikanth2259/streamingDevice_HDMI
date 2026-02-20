/**
 * @file update_service.cpp
 * @brief OTA Update Service - stub for MVP
 */

#include "update_service.hpp"
#include "../common/logger.hpp"

namespace streaming::services {

class UpdateServiceImpl : public IUpdateService {
public:
    device::Result initialize() override { return device::Result::OK; }
    void shutdown() override {}

    device::Result checkForUpdates() override {
        LOG_INFO("Update", "Check for updates - no updates available");
        return device::Result::OK;
    }

    device::Result downloadAndApply(const std::string& update_url,
                                    UpdateProgressCallback progress) override {
        if (progress) progress(0, "Downloading...");
        if (progress) progress(100, "Complete");
        return device::Result::OK;
    }
};

std::unique_ptr<IUpdateService> createUpdateService() {
    return std::make_unique<UpdateServiceImpl>();
}

} // namespace streaming::services
