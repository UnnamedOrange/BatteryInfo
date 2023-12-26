#pragma once

#include <QMap>
#include <QObject>
#include <QVariant>
#include <QVariantList>
#include <QVariantMap>
#include <QtQml/qqmlregistration.h>

#include <battery_info/battery_info_manager.h>

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
                return convert_battery_info(*result);
            } else {
                return QVariant::fromValue(result.error());
            }
        };
        Q_INVOKABLE static QVariant queryBatteryInfoAll() noexcept {
            const auto result = BatteryInfoManager::query_battery_info_all();
            if (result) {
                auto ret = QVariantList{};
                for (const auto& bi : *result) {
                    ret.append(convert_battery_info(bi));
                }
                return ret;
            } else {
                return QVariant::fromValue(result.error());
            }
        }

    private:
        static auto convert_battery_info(const BatteryInfo& bi) -> QVariantMap {
            auto ret = QVariantMap{};

            ret["capacity"] = bi.capacity;
            ret["powerState"] = bi.power_state;
            ret["rate"] = bi.rate;
            ret["voltage"] = bi.voltage;

            ret["designedCapacity"] = bi.designed_capacity;
            ret["fullChargedCapacity"] = bi.full_charged_capacity;
            ret["defaultAlertWarning"] = bi.default_alert_warning;
            ret["defaultAlertLow"] = bi.default_alert_low;

            ret["chemistry"] = QString::fromStdString(bi.chemistry);
            ret["criticalBias"] = bi.critical_bias;
            ret["cycleCount"] = bi.cycle_count;

            return ret;
        }
    };
} // namespace orange::battery
