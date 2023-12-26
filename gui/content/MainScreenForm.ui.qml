
/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts 6.5

Rectangle {
    property alias batterySelector: batterySelector
    property alias textEditCapacity: textEditCapacity
    property alias textEditPowerState: textEditPowerState
    property alias textEditRate: textEditRate
    property alias textEditVoltage: textEditVoltage
    property alias textEditDesignedCapacity: textEditDesignedCapacity
    property alias textEditFullChargedCapacity: textEditFullChargedCapacity
    property alias textEditDefaultAlertWarning: textEditDefaultAlertWarning
    property alias textEditDefaultAlertLow: textEditDefaultAlertLow
    property alias textEditChemistry: textEditChemistry
    property alias textEditCriticalBias: textEditCriticalBias
    property alias textEditCycleCount: textEditCycleCount

    BatterySelector {
        id: batterySelector

        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 4

        button.text: qsTr("Refresh")
    }

    ScrollView {
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.top: batterySelector.bottom
        anchors.topMargin: 8
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8

        contentWidth: availableWidth

        GridLayout {
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0

            columns: 2

            Text {
                text: qsTr("Capacity")

                Layout.maximumWidth: implicitWidth
                Layout.fillHeight: true
            }
            TextEdit {
                id: textEditCapacity

                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Text {
                text: qsTr("Power State")

                Layout.maximumWidth: implicitWidth
                Layout.fillHeight: true
            }
            TextEdit {
                id: textEditPowerState

                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Text {
                text: qsTr("Rate")

                Layout.maximumWidth: implicitWidth
                Layout.fillHeight: true
            }
            TextEdit {
                id: textEditRate

                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Text {
                text: qsTr("Voltage")

                Layout.maximumWidth: implicitWidth
                Layout.fillHeight: true
            }
            TextEdit {
                id: textEditVoltage

                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Text {
                text: qsTr("Designed Capacity")

                Layout.maximumWidth: implicitWidth
                Layout.fillHeight: true
            }
            TextEdit {
                id: textEditDesignedCapacity

                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Text {
                text: qsTr("Full Charged Capacity")

                Layout.maximumWidth: implicitWidth
                Layout.fillHeight: true
            }
            TextEdit {
                id: textEditFullChargedCapacity

                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Text {
                text: qsTr("Default Alert Warning")

                Layout.maximumWidth: implicitWidth
                Layout.fillHeight: true
            }
            TextEdit {
                id: textEditDefaultAlertWarning

                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Text {
                text: qsTr("Default Alert Low")

                Layout.maximumWidth: implicitWidth
                Layout.fillHeight: true
            }
            TextEdit {
                id: textEditDefaultAlertLow

                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Text {
                text: qsTr("Chemistry")

                Layout.maximumWidth: implicitWidth
                Layout.fillHeight: true
            }
            TextEdit {
                id: textEditChemistry

                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Text {
                text: qsTr("Critical Bias")

                Layout.maximumWidth: implicitWidth
                Layout.fillHeight: true
            }
            TextEdit {
                id: textEditCriticalBias

                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Text {
                text: qsTr("Cycle Count")

                Layout.maximumWidth: implicitWidth
                Layout.fillHeight: true
            }
            TextEdit {
                id: textEditCycleCount

                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }
}
