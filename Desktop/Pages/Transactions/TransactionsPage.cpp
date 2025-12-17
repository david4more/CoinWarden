#include "TransactionsPage.h"
#include "ui_TransactionsPage.h"

#include "../../../Backend/Backend.h"
#include "../../../Backend/Managers/CategoriesManager.h"
#include "../../../Backend/Managers/TransactionsManager.h"
#include "../Dialog/MultiSelectDialog.h"
#include "../../../Backend/Modules/Model.h"
#include <QStyledItemDelegate>
#include <QCheckBox>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QSpinBox>
#include <utility>
#include "../../../Backend/Modules/Utils.h"
#include "Managers/CurrenciesManager.h"

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

    types = new QButtonGroup(this);
    types->setExclusive(true);
    types->addButton(ui->noFilter, 0);
    types->addButton(ui->expenseFilter, 1);
    types->addButton(ui->incomeFilter, 2);
    connect(types, &QButtonGroup::idClicked, this, &TransactionsPage::onTypeClicked);

    auto setupCombo = [this](QComboBox* combo, Filter type, const QStringList& items) {
        combo->addItem("All");
        combo->addItem("Multiple...");
        combo->addItems(items);

        connect(combo, &QComboBox::currentIndexChanged, combo, [combo, this, type] (int index){ onComboFilter(combo, type); });
    };
    setupCombo(ui->categoryFilter, Filter::Category, backend->categories()->getNames(CategoryType::All));
    setupCombo(ui->accountFilter, Filter::Account, {"Cash", "User cat", "Doom slayer", "Mother's savings"} );
    setupCombo(ui->currencyFilter, Filter::Currency, backend->currencies()->codes());
}

void TransactionsPage::onCustomFiltersFinished(int result)
{
    if (result != QDialog::Accepted) {
        ui->customFilter->setChecked(false);
        return;
    }

    ui->categoryFilter->setCurrentIndex(0);
    ui->accountFilter->setCurrentIndex(0);
    ui->currencyFilter->setCurrentIndex(0);
}

void TransactionsPage::onComboFilter(QComboBox* combo, Filter filter)
{
    if (combo->currentIndex() == 0) {
        auto filters = proxy->getFilters();
        switch (filter) {
            case Filter::Category: filters.categories.reset(); break;
            case Filter::Account: filters.accounts.reset(); break;
            case Filter::Currency: filters.currencies.reset(); break;
        }
        proxy->useFilters(filters);
        return;
    }

    QStringList items;
    if (combo->currentIndex() == 1) {
        QString title = "Pick ";
        switch (filter) {
        case Filter::Category: title += "categories"; break;
        case Filter::Account: title += "accounts"; break;
        case Filter::Currency: title += "currencies"; break;
        }

        items = MultiSelectDialog::getSelectedItems(
            std::move(title),
            backend->categories()->getNames(static_cast<CategoryType>(types->checkedId())),
            this);

        if (items.empty()) { combo->setCurrentIndex(0); return; }
    }
    else
        items += combo->currentText();

    switch (filter) {
    case Filter::Category:
        proxy->useFilters({.categories = items}); break;
    case Filter::Account:
        proxy->useFilters({.accounts = items}); break;
    case Filter::Currency:
        proxy->useFilters({.currencies = items}); break;
    }
}

void TransactionsPage::onTypeClicked(int index)
{
    CategoryType type = static_cast<CategoryType>(index);
    switch (type) {
    case CategoryType::All:
        proxy->resetFilters(); break;
    case CategoryType::Expense:
        proxy->useFilters({.isExpense = true }); break;
    case CategoryType::Income:
        proxy->useFilters({.isExpense = false }); break;
    }
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






