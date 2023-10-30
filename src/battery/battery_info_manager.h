#pragma once

#include <cstdint>
#include <expected>
#include <optional>
#include <ranges>
#include <vector>

#include "types.h"

namespace orange::battery {
    class BatteryInfoManager {
        using Self = BatteryInfoManager;

    public:
        enum class ErrorType {
            UNKNOWN,
            BATTERY_NOT_EXIST,
            FAIL_TO_QUERY_INFORMATION,
        };

    public:
        static auto query_battery_count() noexcept -> std::optional<uint32_t>;
        static auto query_battery_info(uint32_t idx) noexcept -> std::expected<BatteryInfo, ErrorType>;
        inline static auto query_battery_info_all() noexcept -> std::expected<std::vector<BatteryInfo>, ErrorType> {
            std::vector<BatteryInfo> ret;

            for (uint32_t idx = 0; true; idx++) {
                const auto bi = query_battery_info(idx);
                if (!bi.has_value() && bi.error() == ErrorType::FAIL_TO_QUERY_INFORMATION) {
                    return std::unexpected{ErrorType::FAIL_TO_QUERY_INFORMATION};
                } else if (!bi.has_value() && bi.error() == ErrorType::BATTERY_NOT_EXIST) {
                    break;
                } else if (!bi.has_value()) {
                    return std::unexpected{ErrorType::UNKNOWN};
                }
                ret.push_back(std::move(*bi));
            }
            return ret;
        }
    };
} // namespace orange::battery
