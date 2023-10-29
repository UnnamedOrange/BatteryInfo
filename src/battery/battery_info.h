#pragma once

#include <cstdint>
#include <optional>
#include <ranges>
#include <vector>

#include "types.h"

namespace orange::battery {
    class BatteryInfoManager {
        using Self = BatteryInfoManager;

    public:
        static std::optional<uint32_t> query_battery_count() noexcept;
        static std::optional<BatteryInfo> query_battery_info(uint32_t idx) noexcept;
        inline static std::optional<std::vector<BatteryInfo>> query_battery_info_all() noexcept {
            using Ret = decltype(query_battery_info_all());
            struct Unit {};

            return Self::query_battery_count().and_then([](auto bc) -> Ret {
                std::vector<BatteryInfo> ret;
                for (auto i : std::views::iota(0u, bc)) {
                    const auto result = query_battery_info(i).transform([&](auto&& it) {
                        ret.push_back(std::move(it));
                        return Unit{};
                    });
                    if (!result.has_value()) {
                        return std::nullopt;
                    }
                }
                return std::optional{ret};
            });
        }
    };
} // namespace orange::battery
