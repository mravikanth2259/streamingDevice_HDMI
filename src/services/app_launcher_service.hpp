/**
 * @file app_launcher_service.hpp
 * @brief App Launcher Service - registers and launches streaming apps
 */

#pragma once

#include <streaming_device/types.hpp>
#include <memory>
#include <string>
#include <vector>

namespace streaming::services {

/** App launch request */
struct LaunchRequest {
    std::string app_id;
    std::string content_id;  // Optional: specific content to play
};

/** App launch result */
struct LaunchResult {
    device::AppLaunchStatus status;
    std::string session_id;
    std::string error_message;
};

/**
 * @brief App Launcher Service Interface
 *
 * - Register streaming apps
 * - Launch app by ID
 * - Query app status
 * - Receive events from streaming pipeline
 */
class IAppLauncherService {
public:
    virtual ~IAppLauncherService() = default;

    /** Initialize service */
    virtual device::Result initialize() = 0;

    /** Shutdown */
    virtual void shutdown() = 0;

    /** Register a streaming app */
    virtual device::Result registerApp(const device::AppMetadata& app) = 0;

    /** Get all registered apps */
    virtual std::vector<device::AppMetadata> getApps() const = 0;

    /** Launch app by ID */
    virtual LaunchResult launch(const LaunchRequest& request) = 0;

    /** Stop currently running app */
    virtual device::Result stop(const std::string& session_id) = 0;

    /** Get current app session ID (empty if none) */
    virtual std::string getCurrentSessionId() const = 0;

    /** Check if an app is currently active */
    virtual bool isAppActive() const = 0;
};

std::unique_ptr<IAppLauncherService> createAppLauncherService();

} // namespace streaming::services
