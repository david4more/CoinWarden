#include "NewTransactionForm.h"
#include "ui_NewTransactionForm.h"

#include "../../Backend/Modules/Transaction.h"
#include "../../../Backend/Managers/TransactionsManager.h"
#include "../../../Backend/Managers/CurrenciesManager.h"
#include "../../../Backend/Managers/AccountsManager.h"
#include "../../../Backend/Managers/CategoriesManager.h"
#include "../../Backend/Backend.h"
#include <QButtonGroup>
#include <QMessageBox>
#include <QPushButton>

#include "../Utils.h"

NewTransactionForm::~NewTransactionForm() { delete ui; }
NewTransactionForm::NewTransactionForm(Backend* backend, QWidget* parent) :
    QWidget(parent), ui(new Ui::NewTransactionForm), backend(backend)
{
    ui->setupUi(this);

    connect(ui->add, &QToolButton::clicked, this, &NewTransactionForm::onAddTransaction);
    connect(ui->reset, &QToolButton::clicked, this, [this]
        { if (QMessageBox::question(this, "Confirmation", "Are you sure you want to clear the form?") == QMessageBox::Yes) clear(); });
    connect(ui->back, &QToolButton::clicked, this, [this]{ emit done(); });

    auto* group = new QButtonGroup(this);
    group->setExclusive(true);
    group->addButton(ui->expense);
    group->addButton(ui->income);

    updateData();
}

void NewTransactionForm::updateData()
{
    auto updateCombo = [&](QComboBox* box, QStringList list) {
        box->clear();
        for (const auto& x : list) box->addItem(x);
    };
    updateCombo(ui->currency, backend->currencies()->codes());
    updateCombo(ui->account, backend->accounts()->names());
    updateCombo(ui->category, backend->categories()->getNames(CategoryType::Expense));
}

void NewTransactionForm::clear()
{
    ui->amount->setValue(0);
    ui->currency->setCurrentIndex(0);
    ui->dateTime->setDateTime(QDateTime::currentDateTime());
    ui->category->setCurrentIndex(0);
    ui->account->setCurrentIndex(0);
    ui->note->clear();
}

void NewTransactionForm::onAddTransaction()
{
    Transaction t;
    t.amount = (ui->income->isChecked()) ? ui->amount->value() : -ui->amount->value();
    t.currency = ui->currency->currentText();
    t.dateTime = QDateTime(ui->dateTime->dateTime());
    t.category = backend->categories()->findId(ui->category->currentText(), ui->expense->isChecked());
    t.account = ui->account->currentText();
    t.note = ui->note->text();

    if (!t) {
        Utils::highlightField(ui->amount, ui->amount->value() == 0.f);
        return;
    }

    backend->transactions()->add(std::move(t));
    emit done();
}
