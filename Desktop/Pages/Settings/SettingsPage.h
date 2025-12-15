#pragma once

#include <QWidget>
class Backend;

namespace Ui { class SettingsPage; }

class SettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsPage(Backend* backend, QWidget* parent = nullptr);
    ~SettingsPage() override;

signals:
    void updateUI();
    void updateData();

private:
    Backend* backend;
    Ui::SettingsPage* ui;

    void onResetTransactions();
    void onGetCurrencies();
};