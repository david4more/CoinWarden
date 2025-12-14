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

NewTransactionForm::~NewTransactionForm() { delete ui; }
NewTransactionForm::NewTransactionForm(Backend* backend, QWidget* parent) :
    QWidget(parent), ui(new Ui::NewTransactionForm), backend(backend)
{
    ui->setupUi(this);

    connect(ui->add, &QPushButton::clicked, this, &NewTransactionForm::onAddTransaction);

    auto* group = new QButtonGroup(this);
    group->setExclusive(true);
    group->addButton(ui->expense);
    group->addButton(ui->income);

    ui->addAccount->setIcon(QPixmap(":/budgetIcon"));
    ui->addCategory->setIcon(QPixmap(":/categoryIcon"));
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

void NewTransactionForm::clear()
{
    ui->amount->setValue(0);
    ui->currency->setCurrentIndex(0);
    ui->date->setDate(QDate::currentDate());
    ui->time->setTime(QTime::currentTime());
    ui->category->setCurrentIndex(0);
    ui->account->setCurrentIndex(0);
    ui->note->clear();
}

void NewTransactionForm::onAddTransaction()
{
    Transaction t;
    t.amount = (ui->income->isChecked()) ? ui->amount->value() : -ui->amount->value();
    t.currency = ui->currency->currentText();
    t.dateTime = QDateTime(ui->date->date(), ui->time->time());
    t.category = ui->category->currentText();
    t.account = ui->account->currentText();
    t.note = ui->note->text();

    if (!t) {
        highlightField(ui->amount, ui->amount->value() == 0.f);
        return;
    }

    backend->transactions()->add(std::move(t));
    emit done();
}
