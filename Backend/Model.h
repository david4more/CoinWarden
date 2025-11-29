#pragma once

#include "Modules/utils.h"
#include <QSortFilterProxyModel>
#include "Modules/Transaction.h"

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

class BACKEND_EXPORT TransactionProxy : public QSortFilterProxyModel {
    QStringList categories = {};
    float minAmount = 0.f, maxAmount = 0.f;
    bool useCategoryFilter = false, useMinFilter = false, useMaxFilter = false;
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
public:
    explicit TransactionProxy(QObject* parent) : QSortFilterProxyModel(parent) { setSortRole(Qt::UserRole); setFilterRole(Qt::UserRole); }

    struct Filters {
        bool enabled = true;
        std::optional<float> minAmount;
        std::optional<float> maxAmount;
        std::optional<QStringList> categories;
    };
    void useFilters(Filters f);
};
