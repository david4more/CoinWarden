#include "CustomFiltersForm.h"
#include "ui_CustomFiltersForm.h"

#include "../../Backend/Backend.h"
#include "../../../Backend/Managers/CategoriesManager.h"
#include "../../Backend/Modules/Utils.h"

CustomFiltersForm::CustomFiltersForm(Backend* backend, QWidget* parent) :
    QWidget(parent), ui(new Ui::CustomFiltersForm), backend(backend)
{
    ui->setupUi(this);

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
    updateCombo(ui->incomeCategory, backend->categories()->getNames(CategoryType::Income));
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