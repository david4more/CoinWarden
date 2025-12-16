#pragma once

#include <QWidget>
#include <QDate>
class Backend;
class TransactionModel;
class TransactionProxy;
class QButtonGroup;
class QAbstractButton;

namespace Ui { class TransactionsPage; }

class TransactionsPage : public QWidget
{
    Q_OBJECT

public:
    explicit TransactionsPage(Backend* backend, TransactionModel* model, TransactionProxy* proxy,  QWidget* parent = nullptr);
    ~TransactionsPage() override;

    void updateData();
    void onCustomFiltersFinished(int result);

signals:
    void newTransaction();
    void customFilters();

private:
    enum Filter { No, Expense, Income, Category, Custom };

    int month, year;
    TransactionModel* model;
    TransactionProxy* proxy;
    QButtonGroup* filters;
    Backend* backend;
    Ui::TransactionsPage* ui;

    QPair<QDate, QDate> getDateRange() const;
    void onCustomMonth();
    void onFilterClicked(int index);
    void onCategoryFilterButton();
    void onMonthButton(bool next);
};