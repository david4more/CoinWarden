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
    explicit TransactionsPage(Backend* backend, QWidget* parent = nullptr);
    ~TransactionsPage() override;

    void updateData();

signals:
    void newTransaction();
    void customFilters();

private:
    enum Filter { No, Expense, Income, Category, Custom };

    QDate from, to;
    TransactionModel* model;
    TransactionProxy* proxy;
    QButtonGroup* filters;
    QStringList pickedCategories;
    Backend* backend;
    Ui::TransactionsPage* ui;

    void onFilterClicked(int index);
    void onCategoryFilterButton();
    void onMonthButton(bool next);
};