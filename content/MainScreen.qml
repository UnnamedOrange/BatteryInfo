import QtQuick 6.5
import BatteryInfoManager 1.0

MainScreenForm {
    BatteryInfoManager {
        id: batteryInfoManager
    }

    batterySelector.button.onClicked: {
        let t = batteryInfoManager.queryBatteryInfo(0)
        console.log(JSON.stringify(t))
    }
}
