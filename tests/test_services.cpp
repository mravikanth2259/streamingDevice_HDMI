/**
 * @file test_services.cpp
 * @brief Unit tests for services (App Launcher, UI, Config)
 */

#include "services/app_launcher_service.hpp"
#include "services/ui_service.hpp"
#include "services/config_service.hpp"
#include <cassert>
#include <iostream>

#define ASSERT(cond) do { if (!(cond)) { std::cerr << "FAIL: " << #cond << "\n"; return 1; } } while(0)
#define TEST(name) std::cout << "Test: " << (name) << " ... "; std::cout.flush()

int main() {
    // App Launcher
    TEST("App Launcher - register and launch");
    auto launcher = streaming::services::createAppLauncherService();
    launcher->initialize();
    launcher->registerApp({"netflix", "Netflix", "/icons/n.png", "https://netflix.com", "", true});
    auto apps = launcher->getApps();
    ASSERT(apps.size() == 1);
    ASSERT(apps[0].id == "netflix");

    auto result = launcher->launch({"netflix", ""});
    ASSERT(result.status == streaming::device::AppLaunchStatus::SUCCESS);
    ASSERT(!result.session_id.empty());
    ASSERT(launcher->isAppActive());
    ASSERT(launcher->stop(result.session_id) == streaming::device::Result::OK);
    ASSERT(!launcher->isAppActive());

    result = launcher->launch({"unknown", ""});
    ASSERT(result.status == streaming::device::AppLaunchStatus::NOT_FOUND);
    std::cout << "OK\n";

    // UI Service
    TEST("UI Service - navigation");
    auto ui = streaming::services::createUiService();
    ui->initialize();
    ui->setAppIcons({"a", "b", "c"}, {"A", "B", "C"});
    ASSERT(ui->getFocusedElementId() == "a");
    ui->navigate(streaming::device::RemoteKey::RIGHT);
    ASSERT(ui->getFocusedElementId() == "b");
    ui->navigate(streaming::device::RemoteKey::DOWN);
    ui->select();
    ASSERT(ui->render() == streaming::device::Result::OK);
    std::cout << "OK\n";

    // Config Service
    TEST("Config Service");
    auto config = streaming::services::createConfigService();
    config->initialize();
    ASSERT(config->setString("wifi.ssid", "MyNetwork") == streaming::device::Result::OK);
    std::string ssid;
    ASSERT(config->getString("wifi.ssid", ssid) == streaming::device::Result::OK);
    ASSERT(ssid == "MyNetwork");
    ASSERT(config->setInt("volume", 75) == streaming::device::Result::OK);
    int32_t vol;
    ASSERT(config->getInt("volume", vol) == streaming::device::Result::OK);
    ASSERT(vol == 75);
    std::cout << "OK\n";

    std::cout << "\nAll service tests passed.\n";
    return 0;
}
