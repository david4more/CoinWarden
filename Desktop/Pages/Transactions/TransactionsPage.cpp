#include "TransactionsPage.h"
#include "ui_TransactionsPage.h"

#include "../../Backend/Backend.h"
#include "../../../Backend/Managers/CategoriesManager.h"
#include "../../../Backend/Managers/TransactionsManager.h"
#include "../../../Backend/Managers/CurrenciesManager.h"
#include "../../../Backend/Modules/Model.h"
#include "../Utils.h"
#include <QStyledItemDelegate>
#include <QCheckBox>
#include <QPushButton>
#include <QDialog>
#include <QButtonGroup>

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

TransactionsPage::TransactionsPage(Backend* backend, QWidget* parent) :
    QWidget(parent), ui(new Ui::TransactionsPage), backend(backend)
{
    ui->setupUi(this);

    connect(ui->newTransactionButton, &QPushButton::clicked, this, [this]{ changePage(Page::NewTransaction);});
    connect(ui->prevMonthButton, &QToolButton::clicked, this, [&]{      onMonthButton(false); });
    connect(ui->nextMonthButton, &QToolButton::clicked, this, [&]{      onMonthButton(true); });
    connect(ui->noFilterButton, &QToolButton::clicked, this, [&]{       proxy->useFilters({.enabled = false}); });
    connect(ui->expenseFilterButton, &QToolButton::clicked, this, [&]{  proxy->useFilters({.maxAmount = 0.f}); });
    connect(ui->incomeFilterButton, &QToolButton::clicked, this, [&]{   proxy->useFilters({.minAmount = 0.f}); });
    connect(ui->categoryFilterButton, &QToolButton::clicked, this, &TransactionsPage::onCategoryFilterButton);
    connect(ui->customFilterButton, &QToolButton::clicked, this, [this]{ emit changePage(Page::CustomFilters);});
    connect(ui->dateButton, &QToolButton::clicked, this, [this]{ emit changePage(Page::CustomFilters); });

    ui->transactionsTable->resizeColumnsToContents();
    ui->transactionsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->transactionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->transactionsTable->setAlternatingRowColors(true);
    ui->transactionsTable->setSortingEnabled(true);
    ui->transactionsTable->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->transactionsTable->setItemDelegate(new TransactionDelegate(this));

    model = new TransactionModel(this, backend->currencies()->currencies(), backend->currencies()->symbols());
    proxy = new TransactionProxy(this);
    proxy->setSourceModel(model);
    ui->transactionsTable->setModel(proxy);
    from = QDate(QDate::currentDate().year(), QDate::currentDate().month(), 1);
    to = QDate(QDate::currentDate().year(), QDate::currentDate().month(), QDate::currentDate().daysInMonth());
    updateTransactions();

    auto* group = new QButtonGroup(this);
    group->setExclusive(true);
    group->addButton(ui->noFilterButton);
    group->addButton(ui->expenseFilterButton);
    group->addButton(ui->incomeFilterButton);
    group->addButton(ui->categoryFilterButton);
    group->addButton(ui->customFilterButton);
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
            ui->noFilterButton->click();
    });

    updateCategoriesFilter();
    dialog->setWindowModality(Qt::WindowModal);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

void TransactionsPage::onMonthButton(bool next)
{
    from = from.addMonths(next ? 1 : -1);
    to = QDate(from.year(), from.month(), from.daysInMonth());

    updateTransactions();
}

void TransactionsPage::updateTransactions()
{
    ui->dateButton->setText(from.toString("MMMM yyyy"));
    model->setTransactions(backend->transactions()->get(from, to));
}

TransactionsPage::~TransactionsPage()
{
    delete ui;
}