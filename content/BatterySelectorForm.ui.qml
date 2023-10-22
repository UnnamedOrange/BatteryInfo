
/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 6.5
import QtQuick.Controls 6.5

Item {
    property alias comboBox: comboBox
    property alias button: button

    Button {
        id: button
        text: qsTr("Button")
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
    }

    ComboBox {
        id: comboBox
        anchors.left: parent.left
        anchors.right: button.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.rightMargin: 8
    }
}
