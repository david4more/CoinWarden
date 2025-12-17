#pragma once

#include <QComboBox>
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

    void updateData();
    void onCustomFiltersFinished(int result);

signals:
    void newTransaction();
    void customFilters();

private:
    enum class Filter { Category, Account, Currency, Custom };

    int month, year;
    TransactionModel* model;
    TransactionProxy* proxy;
    QButtonGroup* types;
    Backend* backend;
    Ui::TransactionsPage* ui;

    void onComboFilter(QComboBox* combo, Filter type);
    void onTypeClicked(int index);
    QPair<QDate, QDate> getDateRange() const;
    void onCustomMonth();
    void onMonthButton(bool next);
};