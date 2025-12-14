#pragma once

#include <QWidget>
class Backend;

QT_BEGIN_NAMESPACE

namespace Ui
{
    class SettingsPage;
}

QT_END_NAMESPACE

class SettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsPage(QWidget* parent = nullptr, Backend* backend = nullptr);
    ~SettingsPage() override;

signals:
    void updateUI();

private:
    Backend* backend;
    Ui::SettingsPage* ui;
};