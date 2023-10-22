// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick 6.5
import BatteryInfo

Window {
    id: window
    width: 480
    height: 640

    visible: true
    title: "BatteryInfo"

    MainScreen {
        id: mainScreen
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
    }
}
