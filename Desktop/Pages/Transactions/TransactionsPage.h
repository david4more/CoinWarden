#pragma once

#include <QWidget>
#include <QDate>
class Backend;
class TransactionModel;
class TransactionProxy;
class QButtonGroup;
class QAbstractButton;
class QComboBox;

namespace Ui { class TransactionsPage; }
class TransactionsPage : public QWidget
{
    Q_OBJECT

public:
    explicit TransactionsPage(Backend* backend, TransactionModel* model, TransactionProxy* proxy,  QWidget* parent = nullptr);
    ~TransactionsPage() override;

    void setFilters(QStringList expenseCategories, QStringList incomeCategories, QStringList accounts, QStringList currencies);
    void refresh();
    void onCustomFiltersFinished(int result);

signals:
    void newTransaction();
    void customFilters();
    void updateTransactions(QDate from, QDate to);

private:
    enum class Filter { Category, Account, Currency, Custom };
    QStringList expenseCategories, incomeCategories, accounts, currencies;

    int month, year;
    TransactionModel* model;
    TransactionProxy* proxy;
    QButtonGroup* types;
    Backend* backend;
    Ui::TransactionsPage* ui;

    void updateFilters();
    QStringList getCategories();
    QPair<QDate, QDate> getDateRange() const;
    void onComboFilter(QComboBox* combo, Filter type);
    void onTypeClicked(int index);
    void onCustomMonth();
    void onMonthButton(bool next);
};