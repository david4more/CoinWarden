#include "TransactionsPage.h"
#include "ui_TransactionsPage.h"

#include "../../Backend/Backend.h"
#include "../../../Backend/Managers/CategoriesManager.h"
#include "../../../Backend/Managers/TransactionsManager.h"
#include "../Dialog/MultiSelectDialog.h"
#include "../../../Backend/Modules/Model.h"
#include <QStyledItemDelegate>
#include <QCheckBox>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>
#include <QMessageBox>
#include <QSpinBox>

class TransactionDelegate : public QStyledItemDelegate
{
public:
    explicit TransactionDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        QStyleOptionViewItem opt(option);
        opt.state &= ~QStyle::State_MouseOver;

        opt.font.setFamily("Cascadia Mono");

        QStyledItemDelegate::paint(painter, opt, index);
    }
};

TransactionsPage::~TransactionsPage() { delete ui; }
TransactionsPage::TransactionsPage(Backend* backend, TransactionModel* model, TransactionProxy* proxy, QWidget* parent) :
    QWidget(parent), ui(new Ui::TransactionsPage), backend(backend), model(model), proxy(proxy)
{
    ui->setupUi(this);

    connect(ui->prevMonth, &QToolButton::clicked, this, [&]{ onMonthButton(false); });
    connect(ui->nextMonth, &QToolButton::clicked, this, [&]{ onMonthButton(true); });
    connect(ui->date, &QToolButton::clicked, this, &TransactionsPage::onCustomMonth);

    connect(ui->newTransaction, &QPushButton::clicked, this, [this]{ emit newTransaction(); });

    ui->transactionsTable->resizeColumnsToContents();
    ui->transactionsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->transactionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->transactionsTable->setAlternatingRowColors(true);
    ui->transactionsTable->setSortingEnabled(true);
    ui->transactionsTable->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->transactionsTable->setItemDelegate(new TransactionDelegate(this));

    proxy->setSourceModel(model);
    ui->transactionsTable->setModel(proxy);
    month = QDate::currentDate().month();
    year = QDate::currentDate().year();
    updateData();

    filters = new QButtonGroup(this);
    filters->setExclusive(true);
    filters->addButton(ui->noFilter, 0);
    filters->addButton(ui->expenseFilter, 1);
    filters->addButton(ui->incomeFilter, 2);
    filters->addButton(ui->categoryFilter, 3);
    filters->addButton(ui->customFilter, 4);

    connect(filters, &QButtonGroup::idClicked, this, &TransactionsPage::onFilterClicked);
}

void TransactionsPage::onFilterClicked(int index)
{
    switch (index) {
    case Filter::No:
        proxy->useFilters({.enabled = false}); break;
    case Filter::Expense:
        proxy->useFilters({.isExpense = true }); break;
    case Filter::Income:
        proxy->useFilters({.isExpense = false }); break;
    case Filter::Category:
        onCategoryFilterButton(); break;
    case Filter::Custom:
        emit customFilters(); break;
    }
}

void TransactionsPage::onCategoryFilterButton()
{
    auto categories = MultiSelectDialog::getSelectedItems(
        "Choose categories",
        backend->categories()->getNames(CategoryType::Expense),
        this);

    if (categories.empty()) {
        ui->noFilter->click();
        return;
    }

    proxy->useFilters({.categories = categories});
}

void TransactionsPage::onCustomMonth()
{
    QDialog* dialog = new QDialog(this);

    dialog->setLayout(new QVBoxLayout(dialog));
    dialog->setWindowTitle("Select month");

    QLabel* monthLabel = new QLabel("Month", dialog);
    QSpinBox* month = new QSpinBox(dialog);
    month->setRange(1, 12);
    month->setValue(this->month);

    QLabel* yearLabel = new QLabel("Year", dialog);
    QSpinBox* year = new QSpinBox(dialog);
    year->setRange(1926, 2126);
    year->setValue(this->year);

    QPushButton* button = new QPushButton("Done", dialog);

    dialog->layout()->addWidget(monthLabel);
    dialog->layout()->addWidget(month);
    dialog->layout()->addWidget(yearLabel);
    dialog->layout()->addWidget(year);
    dialog->layout()->addWidget(button);

    connect(button, &QPushButton::clicked, dialog, [dialog, month, year, this] {
        this->month = month->value(); this->year = year->value();

        updateData(); dialog->accept();
    });

    dialog->adjustSize();
    dialog->exec();
}

void TransactionsPage::onMonthButton(bool next)
{
    if (next) {
        if (month == 12) { month = 1; year++; }
        else month++;
    } else {
        if (month == 1) { month = 12; year--; }
        else month--;
    }

    updateData();
}

void TransactionsPage::updateData()
{
    auto range = getDateRange();
    ui->date->setText(range.first.toString("MMMM yyyy"));
    model->setTransactions(backend->transactions()->get(range.first, range.second));
}

void TransactionsPage::onCustomFiltersFinished(int result)
{
    if (result != QDialog::Accepted)
    ui->noFilter->click();
}

QPair<QDate, QDate> TransactionsPage::getDateRange() const
{
    if (month < 1 || month > 12) {
        auto from = QDate(QDate::currentDate().year(), QDate::currentDate().month(), 1);
        auto to = QDate(QDate::currentDate().year(), QDate::currentDate().month(), QDate::currentDate().daysInMonth());

        return {from, to};
    };

    int daysInMonth = QDate(year, month, 1).daysInMonth();
    return {QDate(year, month, 1), QDate(year, month, daysInMonth)};
}






