#pragma once

#include <QMap>
#include <QObject>
#include <QVariant>
#include <QVariantMap>
#include <QtQml/qqmlregistration.h>

#include "../battery/battery_info_manager.h"

namespace orange::battery {
    class BatteryInfoManagerQml : public QObject {
        Q_OBJECT

    public:
        using ErrorType = BatteryInfoManager::ErrorType;
        Q_ENUM(ErrorType)

    public:
        Q_INVOKABLE static QVariant queryBatteryCount() noexcept {
            const auto result = BatteryInfoManager::query_battery_count();
            return result ? *result : QVariant{};
        }
        Q_INVOKABLE static QVariant queryBatteryInfo(uint32_t idx) noexcept {
            const auto result = BatteryInfoManager::query_battery_info(idx);
            if (result) {
                auto ret = QVariantMap{};

                ret["capacity"] = result->capacity;
                ret["powerState"] = result->power_state;
                ret["rate"] = result->rate;
                ret["voltage"] = result->voltage;

                ret["designedCapacity"] = result->designed_capacity;
                ret["fullChargedCapacity"] = result->full_charged_capacity;
                ret["defaultAlertWarning"] = result->default_alert_warning;
                ret["defaultAlertLow"] = result->default_alert_low;

                ret["chemistry"] = QString::fromStdString(result->chemistry);
                ret["criticalBias"] = result->critical_bias;
                ret["cycleCount"] = result->cycle_count;

                return ret;
            } else {
                return QVariant::fromValue(result.error());
            }
        };
    };
} // namespace orange::battery
