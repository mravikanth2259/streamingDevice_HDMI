/**
 * @file app_launcher_service.cpp
 * @brief App Launcher Service implementation
 */

#include "app_launcher_service.hpp"
#include "../common/event_bus.hpp"
#include "../common/logger.hpp"
#include <algorithm>
#include <map>

namespace streaming::services {

class AppLauncherServiceImpl : public IAppLauncherService {
public:
    device::Result initialize() override {
        using namespace common::events;
        common::EventBus::instance().subscribe(APP_STOPPED, [this](const common::Event& e) {
            current_session_.clear();
        });
        return device::Result::OK;
    }

    void shutdown() override {}

    device::Result registerApp(const device::AppMetadata& app) override {
        apps_[app.id] = app;
        LOG_INFO("AppLauncher", "Registered app: ", app.id);
        return device::Result::OK;
    }

    std::vector<device::AppMetadata> getApps() const override {
        std::vector<device::AppMetadata> result;
        for (const auto& p : apps_) result.push_back(p.second);
        return result;
    }

    LaunchResult launch(const LaunchRequest& request) override {
        auto it = apps_.find(request.app_id);
        if (it == apps_.end()) {
            return {device::AppLaunchStatus::NOT_FOUND, "", "App not found"};
        }

        if (!current_session_.empty()) {
            stop(current_session_);
        }

        current_session_ = "sess_" + request.app_id + "_" + std::to_string(++session_counter_);

        common::Event e;
        e.type = common::events::APP_LAUNCHED;
        e.payload = std::make_shared<LaunchRequest>(request);
        common::EventBus::instance().publish(e);

        return {device::AppLaunchStatus::SUCCESS, current_session_, ""};
    }

    device::Result stop(const std::string& session_id) override {
        if (session_id != current_session_) return device::Result::ERROR_NOT_FOUND;
        current_session_.clear();
        common::EventBus::instance().publish(common::Event{
            common::events::APP_STOPPED, std::make_shared<std::string>(session_id)});
        return device::Result::OK;
    }

    std::string getCurrentSessionId() const override { return current_session_; }
    bool isAppActive() const override { return !current_session_.empty(); }

private:
    std::map<std::string, device::AppMetadata> apps_;
    std::string current_session_;
    uint64_t session_counter_{0};
};

std::unique_ptr<IAppLauncherService> createAppLauncherService() {
    return std::make_unique<AppLauncherServiceImpl>();
}

} // namespace streaming::services
