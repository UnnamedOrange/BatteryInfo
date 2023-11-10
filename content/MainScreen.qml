import QtQuick 6.5
import BatteryInfoManager 1.0

MainScreenForm {
    BatteryInfoManager {
        id: batteryInfoManager
    }

    ListModel {
        id: comboBoxModel
    }
    batterySelector.comboBox.model: comboBoxModel

    function updateBatteryInfo(idx) {
        let t = batteryInfoManager.queryBatteryInfo(idx)
        textEditCapacity.text = t["capacity"]
        textEditPowerState.text = t["powerState"]
        textEditRate.text = t["rate"]
        textEditVoltage.text = t["voltage"]
        textEditDesignedCapacity.text = t["designedCapacity"]
        textEditFullChargedCapacity.text = t["fullChargedCapacity"]
        textEditDefaultAlertWarning.text = t["defaultAlertWarning"]
        textEditDefaultAlertLow.text = t["defaultAlertLow"]
        textEditChemistry.text = t["chemistry"]
        textEditCriticalBias.text = t["criticalBias"]
        textEditCycleCount.text = t["cycleCount"]
    }

    Component.onCompleted: {
        let count = batteryInfoManager.queryBatteryCount()
        for (var i = 1; i <= count; i++) {
            comboBoxModel.append({
                                     "text": i
                                 })
        }
        batterySelector.comboBox.currentIndex = 0
    }
    batterySelector.comboBox.onCurrentIndexChanged: {
        updateBatteryInfo(batterySelector.comboBox.currentIndex)
    }
    batterySelector.button.onClicked: {
        updateBatteryInfo(batterySelector.comboBox.currentIndex)
    }
}
