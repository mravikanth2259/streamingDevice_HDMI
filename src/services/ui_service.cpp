/**
 * @file ui_service.cpp
 * @brief UI Service implementation - simple grid launcher
 */

#include "ui_service.hpp"
#include "../common/logger.hpp"
#include <algorithm>

namespace streaming::services {

class UiServiceImpl : public IUiService {
public:
    UiServiceImpl() = default;

    device::Result initialize() override {
        return device::Result::OK;
    }

    void shutdown() override {}

    device::Result render() override {
        // Rendering would use Display HAL - for MVP we log
        LOG_DEBUG("UI", "Render frame, focused: ", focused_index_);
        return device::Result::OK;
    }

    device::Result navigate(device::RemoteKey key) override {
        if (icons_.empty()) return device::Result::OK;

        switch (key) {
            case device::RemoteKey::UP:
                focused_index_ = (focused_index_ - cols_ + icons_.size()) % icons_.size();
                break;
            case device::RemoteKey::DOWN:
                focused_index_ = (focused_index_ + cols_) % icons_.size();
                break;
            case device::RemoteKey::LEFT:
                focused_index_ = (focused_index_ - 1 + icons_.size()) % icons_.size();
                break;
            case device::RemoteKey::RIGHT:
                focused_index_ = (focused_index_ + 1) % icons_.size();
                break;
            default:
                break;
        }
        return device::Result::OK;
    }

    device::Result select() override {
        if (focused_index_ < static_cast<int>(icons_.size())) {
            selected_app_id_ = icons_[focused_index_];
            return device::Result::OK;
        }
        return device::Result::ERROR_INVALID_PARAM;
    }

    device::Result back() override {
        selected_app_id_.clear();
        return device::Result::OK;
    }

    std::string getFocusedElementId() const override {
        if (focused_index_ >= 0 && focused_index_ < static_cast<int>(icons_.size()))
            return icons_[focused_index_];
        return "";
    }

    void setAppIcons(const std::vector<std::string>& app_ids,
                    const std::vector<std::string>& labels) override {
        icons_ = app_ids;
        icon_labels_ = labels;
        focused_index_ = 0;
    }

    std::string getSelectedAppId() const { return selected_app_id_; }

private:
    std::vector<std::string> icons_;
    std::vector<std::string> icon_labels_;
    int focused_index_{0};
    int cols_{4};
    std::string selected_app_id_;
};

std::unique_ptr<IUiService> createUiService() {
    return std::make_unique<UiServiceImpl>();
}

} // namespace streaming::services
