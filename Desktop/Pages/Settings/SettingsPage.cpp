#include "SettingsPage.h"
#include "ui_SettingsPage.h"

#include <QPushButton>
#include <Backend.h>
#include <QMessageBox>

#include "TransactionsManager.h"

SettingsPage::SettingsPage(Backend* backend, QWidget* parent) :
    QWidget(parent), ui(new Ui::SettingsPage), backend(backend)
{
    ui->setupUi(this);

    connect(ui->resetTransactionsBtn, &QPushButton::clicked, this, [this]{
        this->backend->transactions()->setupDefault(); });
}

SettingsPage::~SettingsPage()
{
    delete ui;
}