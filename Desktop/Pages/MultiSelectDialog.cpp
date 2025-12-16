//
// Created by david on 16.12.2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MultiSelectDialog.h" resolved

#include "MultiSelectDialog.h"
#include "ui_MultiSelectDialog.h"


MultiSelectDialog::MultiSelectDialog(QWidget* parent) :
    QDialog(parent), ui(new Ui::MultiSelectDialog)
{
    ui->setupUi(this);
}

MultiSelectDialog::~MultiSelectDialog()
{
    delete ui;
}