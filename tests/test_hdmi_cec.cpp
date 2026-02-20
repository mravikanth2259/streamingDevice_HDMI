/**
 * @file test_hdmi_cec.cpp
 * @brief Unit tests for HDMI-CEC mapping
 */

#include "services/hdmi_cec_service.hpp"
#include <cassert>
#include <iostream>

#define ASSERT(cond) do { if (!(cond)) { std::cerr << "FAIL: " << #cond << "\n"; return 1; } } while(0)
#define TEST(name) std::cout << "Test: " << (name) << " ... "; std::cout.flush()

int main() {
    TEST("HDMI-CEC Service - power and key mapping");
    auto cec = streaming::services::createHdmiCecService();
    ASSERT(cec->initialize() == streaming::device::Result::OK);
    ASSERT(cec->isTvPresent());

    ASSERT(cec->powerOnTv() == streaming::device::Result::OK);
    ASSERT(cec->sendKeyToTv(streaming::device::RemoteKey::VOLUME_UP) == streaming::device::Result::OK);
    ASSERT(cec->sendKeyToTv(streaming::device::RemoteKey::OK) == streaming::device::Result::OK);
    ASSERT(cec->standbyTv() == streaming::device::Result::OK);

    ASSERT(cec->processQueue() == streaming::device::Result::OK);
    std::cout << "OK\n";

    std::cout << "\nHDMI-CEC tests passed.\n";
    return 0;
}
