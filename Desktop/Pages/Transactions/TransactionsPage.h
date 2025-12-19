#pragma once

#include <QWidget>
#include "Utils.h"
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

    void setFilters(QStringList categories, QStringList accounts, QStringList currencies);
    void refresh();
    void onCustomFiltersFinished(int result);

signals:
    void newTransaction();
    void customFilters();
    void updateTransactions(QPair<QDate, QDate> range);
    void requestFilters(TransactionType type);

private:
    enum class Filter { Category, Account, Currency, Custom };
    QStringList categories, accounts, currencies;

    int month, year;
    TransactionType type;

    TransactionModel* model;
    TransactionProxy* proxy;
    QButtonGroup* types;
    Backend* backend;
    Ui::TransactionsPage* ui;

    QPair<QDate, QDate> getDateRange() const;
    void onComboFilter(QComboBox* combo, Filter type);
    void onTypeClicked(int index);
    void onCustomMonth();
    void onMonthButton(bool next);
};