/**
 * @file exceptions.hpp
 * @brief Common exception types for Streaming Device firmware
 */

#pragma once

#include <exception>
#include <string>

namespace streaming::common {

class StreamingDeviceException : public std::exception {
public:
    explicit StreamingDeviceException(const std::string& msg) : msg_(msg) {}
    const char* what() const noexcept override { return msg_.c_str(); }
private:
    std::string msg_;
};

class HalException : public StreamingDeviceException {
public:
    explicit HalException(const std::string& msg) : StreamingDeviceException(msg) {}
};

class NetworkException : public StreamingDeviceException {
public:
    explicit NetworkException(const std::string& msg) : StreamingDeviceException(msg) {}
};

class AuthException : public StreamingDeviceException {
public:
    explicit AuthException(const std::string& msg) : StreamingDeviceException(msg) {}
};

} // namespace streaming::common
