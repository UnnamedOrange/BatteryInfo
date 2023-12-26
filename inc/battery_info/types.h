#pragma once

#include <cstdint>
#include <string>

namespace orange::battery {
    struct BatteryInfo {
        uint32_t capacity;
        uint32_t power_state;
        int32_t rate;
        uint32_t voltage;

        uint32_t designed_capacity;
        uint32_t full_charged_capacity;
        uint32_t default_alert_warning;
        uint32_t default_alert_low;

        std::string chemistry;
        // uint8_t technology; // 1
        // uint32_t capability; // 1 << 31
        uint32_t critical_bias; // optional
        uint32_t cycle_count;   // optional
    };
} // namespace orange::battery
