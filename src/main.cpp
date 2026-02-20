/**
 * @file main.cpp
 * @brief Streaming Device Firmware - Entry point
 *
 * Portable streaming device: HDMI-CEC, Wi-Fi streaming, Bluetooth control,
 * Home page launcher UI.
 */

#include "hal/display_hal.hpp"
#include "hal/input_hal.hpp"
#include "hal/wifi_hal.hpp"
#include "hal/hdmi_cec_hal.hpp"
#include "services/app_launcher_service.hpp"
#include "services/ui_service.hpp"
#include "services/streaming_service.hpp"
#include "services/hdmi_cec_service.hpp"
#include "services/config_service.hpp"
#include "common/event_bus.hpp"
#include "common/logger.hpp"
#include <chrono>
#include <csignal>
#include <iostream>
#include <memory>
#include <thread>

static volatile bool g_running = true;

void signalHandler(int) {
    g_running = false;
}

int main(int argc, char* argv[]) {
    std::signal(SIGINT, signalHandler);

    streaming::common::Logger::instance().setLevel(streaming::common::LogLevel::INFO);
    LOG_INFO("Main", "Streaming Device Firmware starting...");

    // Initialize HAL
    auto display = streaming::hal::createDisplayHal();
    auto input = streaming::hal::createInputHal();
    auto wifi = streaming::hal::createWifiHal();
    auto cec = streaming::hal::createHdmiCecHal();

    if (display->initialize() != streaming::device::Result::OK) {
        LOG_ERROR("Main", "Display init failed");
        return 1;
    }
    if (input->initialize() != streaming::device::Result::OK) {
        LOG_ERROR("Main", "Input init failed");
        return 1;
    }
    if (cec->initialize() != streaming::device::Result::OK) {
        LOG_ERROR("Main", "HDMI-CEC init failed");
    }
    if (wifi->initialize() != streaming::device::Result::OK) {
        LOG_ERROR("Main", "Wi-Fi init failed");
    }

    // Initialize services
    auto app_launcher = streaming::services::createAppLauncherService();
    auto ui = streaming::services::createUiService();
    auto streaming_svc = streaming::services::createStreamingService();
    auto cec_svc = streaming::services::createHdmiCecService();
    auto config = streaming::services::createConfigService();

    app_launcher->initialize();
    config->initialize();
    cec_svc->initialize();
    ui->initialize();
    streaming_svc->initialize();

    // Register streaming apps (example config)
    app_launcher->registerApp({"netflix", "Netflix", "/icons/netflix.png",
        "https://www.netflix.com", "", true});
    app_launcher->registerApp({"prime", "Prime Video", "/icons/prime.png",
        "https://www.primevideo.com", "", true});
    app_launcher->registerApp({"hulu", "Hulu", "/icons/hulu.png",
        "https://www.hulu.com", "", true});
    app_launcher->registerApp({"disney", "Disney+", "/icons/disney.png",
        "https://www.disneyplus.com", "", true});

    auto apps = app_launcher->getApps();
    std::vector<std::string> ids, labels;
    for (const auto& a : apps) {
        ids.push_back(a.id);
        labels.push_back(a.name);
    }
    ui->setAppIcons(ids, labels);

    LOG_INFO("Main", "Entering main loop. Press Ctrl+C to exit.");

    // Main loop - render and poll
    while (g_running) {
        ui->render();
        display->present();

        streaming::hal::InputEvent ev;
        while (input->poll(ev) == streaming::device::Result::OK) {
            if (ev.type == streaming::hal::InputEventType::KEY_DOWN) {
                switch (ev.key) {
                    case streaming::device::RemoteKey::UP:
                    case streaming::device::RemoteKey::DOWN:
                    case streaming::device::RemoteKey::LEFT:
                    case streaming::device::RemoteKey::RIGHT:
                        ui->navigate(ev.key);
                        cec_svc->sendKeyToTv(ev.key);
                        break;
                    case streaming::device::RemoteKey::OK:
                        ui->select();
                        {
                            std::string focused = ui->getFocusedElementId();
                            if (!focused.empty()) {
                                auto result = app_launcher->launch({focused, ""});
                                if (result.status == streaming::device::AppLaunchStatus::SUCCESS) {
                                    streaming_svc->startSession(focused, "", result.session_id);
                                }
                            }
                        }
                        break;
                    case streaming::device::RemoteKey::BACK:
                        if (app_launcher->isAppActive()) {
                            streaming_svc->stopSession();
                            app_launcher->stop(app_launcher->getCurrentSessionId());
                        } else {
                            ui->back();
                        }
                        cec_svc->sendKeyToTv(ev.key);
                        break;
                    case streaming::device::RemoteKey::POWER:
                        cec_svc->standbyTv();
                        break;
                    case streaming::device::RemoteKey::VOLUME_UP:
                    case streaming::device::RemoteKey::VOLUME_DOWN:
                        cec_svc->sendKeyToTv(ev.key);
                        break;
                    default:
                        break;
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));  // ~60 FPS
    }

    LOG_INFO("Main", "Shutting down...");
    streaming_svc->shutdown();
    ui->shutdown();
    app_launcher->shutdown();
    config->shutdown();
    cec_svc->shutdown();
    display->shutdown();
    input->shutdown();
    wifi->shutdown();
    cec->shutdown();

    return 0;
}
