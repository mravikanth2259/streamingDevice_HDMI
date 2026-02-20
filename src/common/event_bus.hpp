/**
 * @file event_bus.hpp
 * @brief In-process message bus for inter-service communication
 */

#pragma once

#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <typeindex>
#include <vector>

namespace streaming::common {

/** Opaque event type - services subscribe by event type string */
using EventType = std::string;

/** Generic event payload (services cast as needed) */
struct Event {
    EventType type;
    std::shared_ptr<void> payload;
};

/** Event handler signature */
using EventHandler = std::function<void(const Event&)>;

/**
 * @brief In-process Event Bus
 * 
 * Decoupled publish/subscribe for services. Thread-safe.
 */
class EventBus {
public:
    static EventBus& instance() {
        static EventBus bus;
        return bus;
    }

    /** Subscribe to event type */
    void subscribe(EventType type, EventHandler handler) {
        std::lock_guard<std::mutex> lock(mutex_);
        subscribers_[type].push_back(handler);
    }

    /** Unsubscribe (by comparing handler - simplified) */
    void unsubscribe(EventType type) {
        std::lock_guard<std::mutex> lock(mutex_);
        subscribers_.erase(type);
    }

    /** Publish event to all subscribers */
    void publish(const Event& event) {
        std::vector<EventHandler> handlers;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            auto it = subscribers_.find(event.type);
            if (it != subscribers_.end()) {
                handlers = it->second;
            }
        }
        for (auto& h : handlers) {
            h(event);
        }
    }

    template<typename T>
    void publish(EventType type, std::shared_ptr<T> payload) {
        Event e;
        e.type = type;
        e.payload = std::static_pointer_cast<void>(payload);
        publish(e);
    }

private:
    std::map<EventType, std::vector<EventHandler>> subscribers_;
    std::mutex mutex_;
};

// Common event type constants
namespace events {
    constexpr const char* INPUT_KEY = "input.key";
    constexpr const char* APP_LAUNCHED = "app.launched";
    constexpr const char* APP_STOPPED = "app.stopped";
    constexpr const char* WIFI_STATE = "wifi.state";
    constexpr const char* CEC_COMMAND = "cec.command";
    constexpr const char* BT_CONTROL = "bt.control";
    constexpr const char* STREAM_ERROR = "stream.error";
    constexpr const char* POWER_STATE = "power.state";
}

} // namespace streaming::common
