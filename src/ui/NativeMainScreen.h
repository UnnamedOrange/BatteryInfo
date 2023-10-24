#pragma once

#include <QObject>

class NativeMainScreen : public QObject {
    Q_OBJECT

public:
    Q_INVOKABLE void on_fresh_button_clicked() {}
};
