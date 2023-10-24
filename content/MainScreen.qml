import QtQuick 6.5
import NativeMainScreen 1.0

MainScreenForm {
    NativeMainScreen{
        id: nativeMainScreen
    }

    batterySelector.button.onClicked: {
        nativeMainScreen.on_fresh_button_clicked()
    }
}
