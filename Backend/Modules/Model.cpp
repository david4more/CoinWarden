#include "Model.h"


// Transaction proxy methods

bool TransactionProxy::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QString cat = sourceModel()->index(sourceRow, 2, sourceParent).data(filterRole()).toString();
    float amt   = sourceModel()->index(sourceRow, 0, sourceParent).data(filterRole()).toFloat();

    if (useCategoryFilter && !categories.contains(cat)) return false;
    if (useMinFilter && amt < minAmount) return false;
    if (useMaxFilter && amt > maxAmount) return false;

    return true;
}

bool TransactionProxy::lessThan(const QModelIndex& left, const QModelIndex& right) const
{
    return QSortFilterProxyModel::lessThan(left, right);
}

void TransactionProxy::useFilters(Filters f)
{
    useMinFilter = false;
    useMaxFilter = false;
    useCategoryFilter = false;

    if (!f.enabled) {
        invalidate();
        return;
    }
    if (f.minAmount) {
        useMinFilter = true;
        minAmount = *f.minAmount;
    }
    if (f.maxAmount) {
        useMaxFilter = true;
        maxAmount = *f.maxAmount;
    }
    if (f.categories) {
        useCategoryFilter = true;
        categories = std::move(*f.categories);
    }

    invalidate();
}



// Transaction model methods

QVariant TransactionModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return {};

    const Transaction& t = transactions[index.row()];

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0: return QString("%1%2%3")
            .arg(symbols[t.currency])
            .arg(t.amount < 0 ? "" : " ")
            .arg(t.amount);
        case 1: return t.dateTime.toString("MMM dd, hh:mm");
        case 2: return t.category;
        case 3: return t.account;
        case 4: return t.note.isEmpty() ? "None" : t.note;
        }
    }
    else if (role == Qt::UserRole) {
        switch (index.column()) {
        case 0: return t.amount / currencies[t.currency];
        case 2: return t.category;
        }
    }


    return {};
}

QVariant TransactionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return {};

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return "Amount";
        case 1: return "Date";
        case 2: return "Category";
        case 3: return "Account";
        case 4: return "Note";
        }
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

void TransactionModel::setTransactions(QVector<Transaction>&& t)
{
    beginResetModel();
    transactions = std::move(t);
    endResetModel();
}







