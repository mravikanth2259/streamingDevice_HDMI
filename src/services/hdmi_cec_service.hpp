/**
 * @file hdmi_cec_service.hpp
 * @brief HDMI-CEC Manager - maps remote/device state to CEC commands
 */

#pragma once

#include <streaming_device/types.hpp>
#include <memory>
#include <queue>

namespace streaming::services {

/**
 * @brief HDMI-CEC Service
 *
 * - Translates remote key → CEC user control
 * - Power on/standby to TV
 * - Command queueing and error handling
 */
class IHdmiCecService {
public:
    virtual ~IHdmiCecService() = default;

    /** Initialize (HAL, mapping table) */
    virtual device::Result initialize() = 0;

    /** Shutdown */
    virtual void shutdown() = 0;

    /** Send remote key to TV via CEC */
    virtual device::Result sendKeyToTv(device::RemoteKey key) = 0;

    /** Power on TV */
    virtual device::Result powerOnTv() = 0;

    /** Standby TV */
    virtual device::Result standbyTv() = 0;

    /** Check if TV is present */
    virtual bool isTvPresent() const = 0;

    /** Process pending command queue */
    virtual device::Result processQueue() = 0;
};

std::unique_ptr<IHdmiCecService> createHdmiCecService();

} // namespace streaming::services
