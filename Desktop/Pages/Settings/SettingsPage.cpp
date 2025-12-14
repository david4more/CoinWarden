#include "SettingsPage.h"
#include "ui_SettingsPage.h"

#include <QPushButton>
#include <Backend.h>
#include <QMessageBox>

#include "../../../Backend/Managers/TransactionsManager.h"
#include "Managers/CurrenciesManager.h"

SettingsPage::SettingsPage(Backend* backend, QWidget* parent) :
    QWidget(parent), ui(new Ui::SettingsPage), backend(backend)
{
    ui->setupUi(this);

    connect(ui->resetTransactions, &QPushButton::clicked, this, &SettingsPage::onResetTransactions);
    connect(ui->getCurrencies, &QPushButton::clicked, this, &SettingsPage::onGetCurrencies);
}

void SettingsPage::onGetCurrencies()
{
    backend->currencies()->requestLatest("EUR,GBP,CHF,PLN,UAH,USD,CAD,JPY,CNY", "EUR");
}

void SettingsPage::onResetTransactions()
{
    if (QMessageBox::question(this,
        "Confirmation", "Are you sure you want to reset all transactions?",
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
        return;

    this->backend->transactions()->setupDefault();
    emit updateUI();
}

SettingsPage::~SettingsPage()
{
    delete ui;
}
