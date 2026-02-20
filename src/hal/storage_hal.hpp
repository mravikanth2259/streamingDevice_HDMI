/**
 * @file storage_hal.hpp
 * @brief Hardware Abstraction Layer for non-volatile storage
 * @copyright 2025 Streaming Device Project
 * 
 * Abstracts settings, app metadata, and user preferences persistence.
 */

#pragma once

#include <streaming_device/types.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace streaming::hal {

/**
 * @brief Storage HAL Interface
 * 
 * Key-value and blob storage for configuration, user preferences,
 * app metadata. Secure persistence with optional encryption.
 */
class IStorageHal {
public:
    virtual ~IStorageHal() = default;

    /** Initialize storage (mount, open partition) */
    virtual device::Result initialize() = 0;

    /** Shutdown and flush */
    virtual device::Result shutdown() = 0;

    /** Read string value by key */
    virtual device::Result read(const std::string& key, std::string& value_out) = 0;

    /** Write string value */
    virtual device::Result write(const std::string& key, const std::string& value) = 0;

    /** Read binary blob */
    virtual device::Result readBytes(const std::string& key,
                                     std::vector<uint8_t>& data_out) = 0;

    /** Write binary blob */
    virtual device::Result writeBytes(const std::string& key,
                                      const std::vector<uint8_t>& data) = 0;

    /** Delete key */
    virtual device::Result remove(const std::string& key) = 0;

    /** Check if key exists */
    virtual bool exists(const std::string& key) const = 0;

    /** List all keys with optional prefix */
    virtual device::Result listKeys(const std::string& prefix,
                                    std::vector<std::string>& keys_out) = 0;

    /** Get total and free space in bytes */
    virtual device::Result getSpace(uint64_t& total_bytes,
                                    uint64_t& free_bytes) = 0;
};

/** Factory for creating platform-specific storage HAL */
std::unique_ptr<IStorageHal> createStorageHal();

} // namespace streaming::hal
