#include "NewTransactionForm.h"
#include "ui_NewTransactionForm.h"

#include "../../Backend/Modules/Transaction.h"
#include "../../../Backend/Managers/TransactionsManager.h"
#include "../../../Backend/Managers/CurrenciesManager.h"
#include "../../../Backend/Managers/AccountsManager.h"
#include "../../../Backend/Managers/CategoriesManager.h"
#include "../../Backend/Backend.h"
#include <QTimer>
#include <QButtonGroup>

NewTransactionForm::NewTransactionForm(Backend* backend, QWidget* parent) :
    QWidget(parent), ui(new Ui::NewTransactionForm), backend(backend)
{
    ui->setupUi(this);

    connect(ui->tExpense, &QToolButton::clicked, this, [this]{  });

    auto* group = new QButtonGroup(this);
    group->setExclusive(true);
    group->addButton(ui->tExpense);
    group->addButton(ui->tIncome);

    ui->tAddAccount->setIcon(QPixmap(":/budgetIcon"));
    ui->tAddCategory->setIcon(QPixmap(":/categoryIcon"));
}

void NewTransactionForm::updateData()
{
    auto updateCombo = [&](QComboBox* box, QStringList list) {
        box->clear();
        for (const auto& x : list) box->addItem(x);
    };
    updateCombo(ui->tCurrency, backend->currencies()->codes());
    updateCombo(ui->tAccount, backend->accounts()->names());
    updateCombo(ui->tCategory, backend->categories()->getNames(CategoryType::Expense));
}

void NewTransactionForm::highlightField(QWidget* widget, bool condition)
{
    if (!condition) return;

    QTimer* timer = widget->findChild<QTimer*>("errorTimer");
    if (!timer) {
        timer = new QTimer(widget);
        timer->setObjectName("errorTimer");
        timer->setSingleShot(true);
        connect(timer, &QTimer::timeout, [widget]() {
            if (widget) widget->setStyleSheet("");
        });
    }

    widget->setStyleSheet("background-color: #5a1f1f;");
    timer->stop();
    timer->start(2000);
}

void NewTransactionForm::clearTransactionForm()
{
    ui->tAmount->setValue(0);
    ui->tCurrency->setCurrentIndex(0);
    ui->tDate->setDate(QDate::currentDate());
    ui->tTime->setTime(QTime::currentTime());
    ui->tCategory->setCurrentIndex(0);
    ui->tAccount->setCurrentIndex(0);
    ui->tNote->clear();
}

void NewTransactionForm::onAddTransaction()
{
    Transaction t;
    t.amount = (ui->tIncome->isChecked()) ? ui->tAmount->value() : -ui->tAmount->value();
    t.currency = ui->tCurrency->currentText();
    t.dateTime = QDateTime(ui->tDate->date(), ui->tTime->time());
    t.category = ui->tCategory->currentText();
    t.account = ui->tAccount->currentText();
    t.note = ui->tNote->text();

    if (!t) {
        highlightField(ui->tAmount, ui->tAmount->value() == 0.f);
        return;
    }

    backend->transactions()->add(std::move(t));

    // updateTransactions();
    // changePage(Page::Transactions);
}

NewTransactionForm::~NewTransactionForm()
{
    delete ui;
}