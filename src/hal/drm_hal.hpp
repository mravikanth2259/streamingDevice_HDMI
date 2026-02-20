/**
 * @file drm_hal.hpp
 * @brief DRM and content protection hooks for HEVC/AV1/ProRes
 * @copyright 2025 Streaming Device Project
 *
 * Secure key handling and decryption hooks. Implementations integrate
 * with platform-specific Widevine, PlayReady, or FairPlay.
 */

#pragma once

#include <streaming_device/types.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace streaming::hal {

/** DRM key request/response (opaque - implementation-specific) */
struct DrmKeyRequest {
    std::string key_system;  /* e.g., "com.widevine.alpha" */
    std::vector<uint8_t> init_data;
    std::string session_id;
};

struct DrmKeyResponse {
    bool success{false};
    std::vector<uint8_t> key_data;
    std::string error_message;
};

/**
 * @brief DRM HAL Interface (optional hook)
 *
 * Decoders can call this for protected content decryption.
 * Secure key handling must be implementation-specific.
 */
class IDrmHal {
public:
    virtual ~IDrmHal() = default;

    virtual device::Result initialize() = 0;
    virtual device::Result shutdown() = 0;

    /** Request decryption keys for init_data */
    virtual device::Result requestKeys(const DrmKeyRequest& request,
                                       DrmKeyResponse& response) = 0;

    /** Release session */
    virtual device::Result releaseSession(const std::string& session_id) = 0;

    /** Check if content is protected (encrypted) */
    virtual bool isContentProtected(const std::vector<uint8_t>& init_data) const = 0;
};

/** Null DRM implementation for unprotected content */
std::unique_ptr<IDrmHal> createDrmHal();

} // namespace streaming::hal
