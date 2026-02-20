#include "mock_storage_driver.hpp"

namespace streaming::drivers::mock {

device::Result MockStorageDriver::initialize() { return device::Result::OK; }
device::Result MockStorageDriver::shutdown() { return device::Result::OK; }

device::Result MockStorageDriver::read(const std::string& key, std::string& value_out) {
    auto it = string_store_.find(key);
    if (it == string_store_.end()) return device::Result::ERROR_NOT_FOUND;
    value_out = it->second;
    return device::Result::OK;
}

device::Result MockStorageDriver::write(const std::string& key, const std::string& value) {
    string_store_[key] = value;
    return device::Result::OK;
}

device::Result MockStorageDriver::readBytes(const std::string& key, std::vector<uint8_t>& data_out) {
    auto it = blob_store_.find(key);
    if (it == blob_store_.end()) return device::Result::ERROR_NOT_FOUND;
    data_out = it->second;
    return device::Result::OK;
}

device::Result MockStorageDriver::writeBytes(const std::string& key, const std::vector<uint8_t>& data) {
    blob_store_[key] = data;
    return device::Result::OK;
}

device::Result MockStorageDriver::remove(const std::string& key) {
    string_store_.erase(key);
    blob_store_.erase(key);
    return device::Result::OK;
}

bool MockStorageDriver::exists(const std::string& key) const {
    return string_store_.count(key) || blob_store_.count(key);
}

device::Result MockStorageDriver::listKeys(const std::string& prefix, std::vector<std::string>& keys_out) {
    for (const auto& p : string_store_) {
        if (p.first.find(prefix) == 0) keys_out.push_back(p.first);
    }
    for (const auto& p : blob_store_) {
        if (p.first.find(prefix) == 0) keys_out.push_back(p.first);
    }
    return device::Result::OK;
}

device::Result MockStorageDriver::getSpace(uint64_t& total_bytes, uint64_t& free_bytes) {
    total_bytes = 1024ULL * 1024 * 1024;
    free_bytes = 512ULL * 1024 * 1024;
    return device::Result::OK;
}

} // namespace streaming::drivers::mock
