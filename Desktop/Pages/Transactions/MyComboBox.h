#pragma once

#include <QComboBox>

class MyComboBox : public QComboBox {
    Q_OBJECT

signals:
    void aboutToShowPopup();

public:
    MyComboBox(QWidget* parent = nullptr) : QComboBox(parent) {}
    void showPopup() override { emit aboutToShowPopup(); QComboBox::showPopup(); }
};
