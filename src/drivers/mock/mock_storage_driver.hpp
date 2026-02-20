#pragma once

#include "../../hal/storage_hal.hpp"
#include <map>

namespace streaming::drivers::mock {

class MockStorageDriver : public hal::IStorageHal {
public:
    device::Result initialize() override;
    device::Result shutdown() override;
    device::Result read(const std::string& key, std::string& value_out) override;
    device::Result write(const std::string& key, const std::string& value) override;
    device::Result readBytes(const std::string& key, std::vector<uint8_t>& data_out) override;
    device::Result writeBytes(const std::string& key, const std::vector<uint8_t>& data) override;
    device::Result remove(const std::string& key) override;
    bool exists(const std::string& key) const override;
    device::Result listKeys(const std::string& prefix, std::vector<std::string>& keys_out) override;
    device::Result getSpace(uint64_t& total_bytes, uint64_t& free_bytes) override;

private:
    std::map<std::string, std::string> string_store_;
    std::map<std::string, std::vector<uint8_t>> blob_store_;
};

} // namespace streaming::drivers::mock
