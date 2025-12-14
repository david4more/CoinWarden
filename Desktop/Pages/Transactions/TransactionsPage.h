#pragma once

#include <QWidget>
#include <QDate>
class Backend;
class TransactionModel;
class TransactionProxy;

namespace Ui { class TransactionsPage; }

class TransactionsPage : public QWidget
{
    Q_OBJECT

public:
    explicit TransactionsPage(Backend* backend, QWidget* parent = nullptr);
    ~TransactionsPage() override;

signals:
    void changePage(int index);

private:
    QDate from, to;
    TransactionModel* model;
    TransactionProxy* proxy;
    QStringList pickedCategories;
    Backend* backend;
    Ui::TransactionsPage* ui;

    void onCategoryFilterButton();
    void onMonthButton(bool next);
    void updateTransactions();
};