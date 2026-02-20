/**
 * @file test_runner.cpp
 * @brief Combined test runner for HAL, services, HDMI-CEC, Bluetooth
 */

#include "hal/display_hal.hpp"
#include "hal/input_hal.hpp"
#include "hal/storage_hal.hpp"
#include "hal/hdmi_cec_hal.hpp"
#include "drivers/mock/mock_input_driver.hpp"
#include "services/app_launcher_service.hpp"
#include "services/ui_service.hpp"
#include "services/config_service.hpp"
#include "services/hdmi_cec_service.hpp"
#include "services/bluetooth_control_service.hpp"
#include "services/codec_service.hpp"
#include "services/container_service.hpp"
#include "services/stream_pipeline_service.hpp"
#include "hal/container_hal.hpp"
#include <cassert>
#include <iostream>

#define ASSERT(cond) do { if (!(cond)) { std::cerr << "FAIL: " << #cond << "\n"; ++failures; } else { ++passed; } } while(0)
#define TEST(name) std::cout << "Test: " << (name) << " ... "; std::cout.flush()
#define TEST_END() std::cout << (failures ? "FAIL\n" : "OK\n")

static int passed = 0, failures = 0;

void run_hal_tests() {
    std::cout << "\n=== HAL Tests ===\n";

    TEST("Display init");
    auto display = streaming::hal::createDisplayHal();
    ASSERT(display->initialize() == streaming::device::Result::OK);
    ASSERT(display->isConnected());
    ASSERT(display->getDisplayMode().width == 1920);
    ASSERT(display->clear() == streaming::device::Result::OK);
    TEST_END();

    TEST("Input init and inject");
    auto input = streaming::hal::createInputHal();
    ASSERT(input->initialize() == streaming::device::Result::OK);
    auto* mock_input = dynamic_cast<streaming::drivers::mock::MockInputDriver*>(input.get());
    ASSERT(mock_input != nullptr);
    mock_input->injectKey(streaming::device::RemoteKey::UP, streaming::hal::InputEventType::KEY_DOWN);
    ASSERT(input->getLastKey() == streaming::device::RemoteKey::UP);
    TEST_END();

    TEST("Storage read/write");
    auto storage = streaming::hal::createStorageHal();
    ASSERT(storage->initialize() == streaming::device::Result::OK);
    ASSERT(storage->write("test.key", "test.value") == streaming::device::Result::OK);
    std::string val;
    ASSERT(storage->read("test.key", val) == streaming::device::Result::OK);
    ASSERT(val == "test.value");
    ASSERT(storage->remove("test.key") == streaming::device::Result::OK);
    TEST_END();
}

void run_service_tests() {
    std::cout << "\n=== Service Tests ===\n";

    TEST("App Launcher");
    auto launcher = streaming::services::createAppLauncherService();
    launcher->initialize();
    launcher->registerApp({"netflix", "Netflix", "/icons/n.png", "https://netflix.com", "", true});
    auto apps = launcher->getApps();
    ASSERT(apps.size() == 1);
    auto result = launcher->launch({"netflix", ""});
    ASSERT(result.status == streaming::device::AppLaunchStatus::SUCCESS);
    ASSERT(launcher->stop(result.session_id) == streaming::device::Result::OK);
    result = launcher->launch({"unknown", ""});
    ASSERT(result.status == streaming::device::AppLaunchStatus::NOT_FOUND);
    TEST_END();

    TEST("UI Service");
    auto ui = streaming::services::createUiService();
    ui->initialize();
    ui->setAppIcons({"a", "b", "c"}, {"A", "B", "C"});
    ASSERT(ui->getFocusedElementId() == "a");
    ui->navigate(streaming::device::RemoteKey::RIGHT);
    ASSERT(ui->getFocusedElementId() == "b");
    ASSERT(ui->render() == streaming::device::Result::OK);
    TEST_END();

    TEST("Config Service");
    auto config = streaming::services::createConfigService();
    config->initialize();
    ASSERT(config->setString("wifi.ssid", "MyNetwork") == streaming::device::Result::OK);
    std::string ssid;
    ASSERT(config->getString("wifi.ssid", ssid) == streaming::device::Result::OK);
    ASSERT(ssid == "MyNetwork");
    TEST_END();
}

void run_cec_tests() {
    std::cout << "\n=== HDMI-CEC Tests ===\n";

    TEST("HDMI-CEC Service");
    auto cec = streaming::services::createHdmiCecService();
    ASSERT(cec->initialize() == streaming::device::Result::OK);
    ASSERT(cec->isTvPresent());
    ASSERT(cec->powerOnTv() == streaming::device::Result::OK);
    ASSERT(cec->sendKeyToTv(streaming::device::RemoteKey::VOLUME_UP) == streaming::device::Result::OK);
    ASSERT(cec->standbyTv() == streaming::device::Result::OK);
    TEST_END();
}

void run_codec_container_tests() {
    std::cout << "\n=== Codec & Container Tests ===\n";

    TEST("CodecService - register and create decoder");
    auto codec_svc = streaming::services::createCodecService();
    codec_svc->initialize();
    ASSERT(codec_svc->isSupported(streaming::media::VideoCodec::H265_HEVC));
    ASSERT(codec_svc->isSupported(streaming::media::VideoCodec::AV1));
    streaming::media::VideoTrackInfo track;
    track.codec = streaming::media::VideoCodec::H265_HEVC;
    track.width = 1920;
    track.height = 1080;
    auto dec = codec_svc->createDecoder(track, true);
    ASSERT(dec != nullptr);
    auto caps = dec->getCapabilities();
    ASSERT(caps.codec == streaming::media::VideoCodec::H265_HEVC);
    streaming::media::EncodedPacket pkt;
    pkt.data = {0x00, 0x01, 0x02};
    pkt.timing.pts = 0;
    auto result = dec->decodeFrame(pkt);
    ASSERT(result.status == streaming::device::Result::OK);
    ASSERT(result.frame_ready);
    TEST_END();

    TEST("ContainerService - open and get tracks");
    auto container_svc = streaming::services::createContainerService();
    container_svc->initialize();
    ASSERT(container_svc->open("test.mp4") == streaming::device::Result::OK);
    auto tracks = container_svc->getTracks();
    ASSERT(tracks.size() >= 1);
    ASSERT(container_svc->getDurationUs() > 0);
    ASSERT(container_svc->close() == streaming::device::Result::OK);
    TEST_END();

    TEST("StreamPipeline - open and play");
    auto pipeline = streaming::services::createStreamPipeline();
    pipeline->initialize();
    ASSERT(pipeline->open("video.mp4") == streaming::device::Result::OK);
    ASSERT(pipeline->play() == streaming::device::Result::OK);
    ASSERT(pipeline->getState() == streaming::services::PipelineState::PLAYING);
    ASSERT(pipeline->seek(5000000) == streaming::device::Result::OK);
    ASSERT(pipeline->stop() == streaming::device::Result::OK);
    TEST_END();
}

void run_bluetooth_tests() {
    std::cout << "\n=== Bluetooth Control Tests ===\n";

    TEST("Bluetooth Control Service");
    auto bt = streaming::services::createBluetoothControlService();
    ASSERT(bt->initialize() == streaming::device::Result::OK);
    ASSERT(bt->sendStatus({0x01, 0x02}) == streaming::device::Result::OK);
    TEST_END();
}

int main() {
    run_hal_tests();
    run_service_tests();
    run_cec_tests();
    run_codec_container_tests();
    run_bluetooth_tests();

    std::cout << "\n========================================\n";
    std::cout << "Passed: " << passed << ", Failed: " << failures << "\n";
    return failures ? 1 : 0;
}
