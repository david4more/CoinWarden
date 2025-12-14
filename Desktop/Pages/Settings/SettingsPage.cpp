#include "SettingsPage.h"
#include "ui_SettingsPage.h"

#include <QPushButton>
#include <Backend.h>
#include <QMessageBox>

#include "TransactionsManager.h"

SettingsPage::SettingsPage(QWidget* parent, Backend* backend) :
    QWidget(parent), ui(new Ui::SettingsPage)
{
    ui->setupUi(this);


    connect(ui->resetTransactionsBtn, &QPushButton::clicked, this, [this]{ QMessageBox::information(this, "you did it!", "hehe");
        this->backend->transactions()->setupDefault(); });
    this->backend = backend;
}

SettingsPage::~SettingsPage()
{
    delete ui;
}