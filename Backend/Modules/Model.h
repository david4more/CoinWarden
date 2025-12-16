#pragma once

#include "Utils.h"
#include <QSortFilterProxyModel>
#include "Transaction.h"

class Transaction;
class BACKEND_EXPORT TransactionModel : public QAbstractTableModel
{
    Q_OBJECT
    QVector<Transaction> transactions;
    QMap<QString, double> currencies;
    QMap<QString, QChar> symbols;
public:
    explicit TransactionModel(QObject* parent, QMap<QString, double> currencies, QMap<QString, QChar> symbols)
    : QAbstractTableModel(parent), currencies(currencies), symbols(symbols) {}

    void setTransactions(QVector<Transaction>&& t);
    int rowCount(const QModelIndex&) const override { return transactions.size(); }
    int columnCount(const QModelIndex&) const override { return 5; }
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};



class BACKEND_EXPORT TransactionProxy : public QSortFilterProxyModel
{
    bool enabled = false;

    std::optional<bool> isExpense = std::nullopt;
    QStringList categories = {}, accounts = {}, currencies = {};
    float maxAmount = 0.f;
    QString note = {};
    QDate from = {}, to = {};

    void resetValues();
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

public:
    explicit TransactionProxy(QObject* parent) : QSortFilterProxyModel(parent) { setSortRole(Qt::UserRole); setFilterRole(Qt::UserRole); }

    struct Filters {
        bool enabled = true;
        std::optional<bool> isExpense;
        std::optional<float> maxAmount;
        std::optional<QStringList> currencies;
        std::optional<QStringList> categories;
        std::optional<QStringList> accounts;
        std::optional<QDate> from, to;
        std::optional<QString> note;
    };
    void useFilters(Filters f);
};
