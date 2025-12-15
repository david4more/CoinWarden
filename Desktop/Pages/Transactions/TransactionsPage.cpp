#include "TransactionsPage.h"
#include "ui_TransactionsPage.h"

#include "../../Backend/Backend.h"
#include "../../../Backend/Managers/CategoriesManager.h"
#include "../../../Backend/Managers/TransactionsManager.h"
#include "../../../Backend/Managers/CurrenciesManager.h"
#include "../../../Backend/Modules/Model.h"
#include <QStyledItemDelegate>
#include <QCheckBox>
#include <QPushButton>
#include <QButtonGroup>
#include <QMessageBox>

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
TransactionsPage::TransactionsPage(Backend* backend, QWidget* parent) :
    QWidget(parent), ui(new Ui::TransactionsPage), backend(backend)
{
    ui->setupUi(this);

    connect(ui->prevMonth, &QToolButton::clicked, this, [&]{ onMonthButton(false); });
    connect(ui->nextMonth, &QToolButton::clicked, this, [&]{ onMonthButton(true); });

    connect(ui->newTransaction, &QPushButton::clicked, this, [this]{ emit newTransaction(); });

    ui->transactionsTable->resizeColumnsToContents();
    ui->transactionsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->transactionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->transactionsTable->setAlternatingRowColors(true);
    ui->transactionsTable->setSortingEnabled(true);
    ui->transactionsTable->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->transactionsTable->setItemDelegate(new TransactionDelegate(this));

    model = new TransactionModel(this, backend->currencies()->rates(), backend->currencies()->symbols());
    proxy = new TransactionProxy(this);
    proxy->setSourceModel(model);
    ui->transactionsTable->setModel(proxy);
    from = QDate(QDate::currentDate().year(), QDate::currentDate().month(), 1);
    to = QDate(QDate::currentDate().year(), QDate::currentDate().month(), QDate::currentDate().daysInMonth());
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
        proxy->useFilters({.maxAmount = 0.f}); break;
    case Filter::Income:
        proxy->useFilters({.minAmount = 0.f}); break;
    case Filter::Category:
        onCategoryFilterButton(); break;
    case Filter::Custom:
        emit customFilters(); break;
    }
}

void TransactionsPage::onCategoryFilterButton()
{
    QDialog* dialog = new QDialog(this);
    QVBoxLayout* layout = new QVBoxLayout(dialog);

    for (const auto& item : backend->categories()->getNames()) {
        QCheckBox* box = new QCheckBox(item, dialog);
        box->setChecked(pickedCategories.contains(item));
        layout->addWidget(box);
    }

    QPushButton* button = new QPushButton("Done", dialog);
    layout->addWidget(button);

    // Update pickedCategories whenever a checkbox is clicked
    auto updateCategoriesFilter = [dialog, this]{
        pickedCategories.clear();
        for (auto* box : dialog->findChildren<QCheckBox*>()) {
            if (box->checkState() == Qt::Checked)
                pickedCategories.append(box->text());
        }

        proxy->useFilters({.categories = pickedCategories});
    };

    for (auto* box : dialog->findChildren<QCheckBox*>())
        connect(box, &QCheckBox::clicked, dialog, updateCategoriesFilter);

    connect(button, &QPushButton::clicked, dialog, [dialog]{ dialog->accept(); });

    connect(dialog, &QDialog::finished, this, [=](int result) {
        if ((result == QDialog::Accepted && pickedCategories.empty()) || result == QDialog::Rejected)
            ui->noFilter->click();
    });

    updateCategoriesFilter();
    dialog->setWindowModality(Qt::WindowModal);
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    dialog->adjustSize();
    dialog->show();
}

void TransactionsPage::onMonthButton(bool next)
{
    from = from.addMonths(next ? 1 : -1);
    to = QDate(from.year(), from.month(), from.daysInMonth());

    updateData();
}

void TransactionsPage::updateData()
{
    ui->date->setText(from.toString("MMMM yyyy"));
    model->setTransactions(backend->transactions()->get(from, to));
}







