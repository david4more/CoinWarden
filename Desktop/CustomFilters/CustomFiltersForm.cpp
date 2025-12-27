#include "CustomFiltersForm.h"
#include "ui_CustomFiltersForm.h"

#include "../../Backend/Modules/Model.h"
#include "../MultiSelect/MultiSelectDialog.h"

#include <QButtonGroup>

CustomFiltersForm::CustomFiltersForm(TransactionProxy* proxy, QWidget* parent) :
    QDialog(parent), ui(new Ui::CustomFiltersForm), proxy(proxy)
{
    ui->setupUi(this);

    QButtonGroup* buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->expense);
    buttonGroup->addButton(ui->income);
    buttonGroup->setExclusive(true);

    connect(buttonGroup, &QButtonGroup::idClicked, this, &CustomFiltersForm::onButtonGroupClicked);
    connect(ui->apply, &QPushButton::clicked, this, &CustomFiltersForm::onApplyCustomFilters);
}

void CustomFiltersForm::onButtonGroupClicked(int index)
{
    ui->category->clear();
    //for (const auto& x : list) box->addItem(x);
}

void CustomFiltersForm::updateData()
{
}

void CustomFiltersForm::onApplyCustomFilters()
{
    TransactionProxy::Filters filters;

    for (auto x : MultiSelectDialog::getSelectedItems("Hello from debug",
        {"hehe", "not hehe", "real hehe", "a", "b", "c", "d", "e", "f", "g", "h", "j" }, this))
        qDebug() << x;


    proxy->useFilters( filters );
    this->accept();
}

CustomFiltersForm::~CustomFiltersForm()
{
    delete ui;
}
