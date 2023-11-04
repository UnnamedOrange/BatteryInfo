#pragma once

#include <QObject>
#include <QVariant>
#include <QtQml/qqmlregistration.h>

#include "../battery/battery_info_manager.h"

namespace orange::battery {
    class BatteryInfoManagerQml : public QObject {
        Q_OBJECT

    public:
        Q_INVOKABLE static QVariant query_battery_count() noexcept {
            const auto result = BatteryInfoManager::query_battery_count();
            return result ? *result : QVariant{};
        }
    };
} // namespace orange::battery
