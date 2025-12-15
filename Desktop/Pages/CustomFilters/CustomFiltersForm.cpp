#include "CustomFiltersForm.h"
#include "ui_CustomFiltersForm.h"

#include "../../Backend/Backend.h"
#include "../../../Backend/Managers/CategoriesManager.h"
#include "../../Backend/Modules/Utils.h"

#include <QButtonGroup>

CustomFiltersForm::CustomFiltersForm(Backend* backend, QWidget* parent) :
    QDialog(parent), ui(new Ui::CustomFiltersForm), backend(backend)
{
    ui->setupUi(this);

    QButtonGroup* buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->expense);
    buttonGroup->addButton(ui->income);
    buttonGroup->setExclusive(true);
}

void CustomFiltersForm::setupButtonGroups()
{
}

void CustomFiltersForm::updateData()
{
    auto updateCombo = [&](QComboBox* box, QStringList list) {
        box->clear();
        for (const auto& x : list) box->addItem(x);
    };

    updateCombo(ui->expenseCategory, backend->categories()->getNames(CategoryType::Expense));
}

void CustomFiltersForm::clearForm()
{
    ;
}

void CustomFiltersForm::onAddCategory()
{
    ;
}

void CustomFiltersForm::onApplyCustomFilters()
{
    ;
}

CustomFiltersForm::~CustomFiltersForm()
{
    delete ui;
}
