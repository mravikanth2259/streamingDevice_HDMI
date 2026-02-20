/**
 * @file media_hal_factory.cpp
 * @brief Factory for container parser and DRM HAL (mock implementations)
 */

#include "container_hal.hpp"
#include "drm_hal.hpp"
#include "../drivers/mock/mock_container_parser.hpp"

namespace streaming::hal {

std::unique_ptr<IContainerParser> createContainerParser() {
    return std::make_unique<drivers::mock::MockContainerParser>();
}

/** Null DRM implementation - no content protection */
class NullDrmHal : public IDrmHal {
public:
    device::Result initialize() override { return device::Result::OK; }
    device::Result shutdown() override { return device::Result::OK; }

    device::Result requestKeys(const DrmKeyRequest&, DrmKeyResponse& response) override {
        response.success = false;
        response.error_message = "DRM not implemented";
        return device::Result::ERROR_NOT_SUPPORTED;
    }

    device::Result releaseSession(const std::string&) override { return device::Result::OK; }

    bool isContentProtected(const std::vector<uint8_t>&) const override { return false; }
};

std::unique_ptr<IDrmHal> createDrmHal() {
    return std::make_unique<NullDrmHal>();
}

}  // namespace streaming::hal
