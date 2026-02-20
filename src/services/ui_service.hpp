/**
 * @file ui_service.hpp
 * @brief UI Service - home page launcher UI, navigation, rendering
 */

#pragma once

#include <streaming_device/types.hpp>
#include <memory>
#include <string>
#include <vector>

namespace streaming::services {

/** UI element (app icon, focus state) */
struct UIElement {
    std::string id;
    std::string label;
    int x{0}, y{0}, w{0}, h{0};
    bool focused{false};
};

/**
 * @brief UI Service Interface
 *
 * Renders home page with app icons. Handles navigation (focus, selection).
 */
class IUiService {
public:
    virtual ~IUiService() = default;

    /** Initialize UI (display, layout) */
    virtual device::Result initialize() = 0;

    /** Shutdown */
    virtual void shutdown() = 0;

    /** Render current frame */
    virtual device::Result render() = 0;

    /** Handle navigation key (UP, DOWN, LEFT, RIGHT) */
    virtual device::Result navigate(device::RemoteKey key) = 0;

    /** Handle select/OK */
    virtual device::Result select() = 0;

    /** Handle back */
    virtual device::Result back() = 0;

    /** Get currently focused element ID */
    virtual std::string getFocusedElementId() const = 0;

    /** Set app icons for launcher (from App Launcher service) */
    virtual void setAppIcons(const std::vector<std::string>& app_ids,
                             const std::vector<std::string>& labels) = 0;
};

std::unique_ptr<IUiService> createUiService();

} // namespace streaming::services
