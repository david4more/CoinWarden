#include "Model.h"


// Transaction proxy methods

void TransactionProxy::resetValues()
{
    enabled = false;

    isExpense = -1;
    categories = {}, accounts = {}, currencies = {};
    maxAmount = 0.f;
    note = {};
    from = {}, to = {};
}

void TransactionProxy::useFilters(Filters f)
{
    if (!f.enabled) {
        resetValues();
        invalidate();
        return;
    }
    enabled = true;

    if (f.isExpense) {
        isExpense = *f.isExpense ? 1 : 0;
    }
    if (f.maxAmount) {
        maxAmount = *f.maxAmount;
    }
    if (f.categories) {
        categories = std::move(*f.categories);
    }
    if (f.accounts) {
        accounts = std::move(*f.accounts);
    }
    if (f.currencies) {
        accounts = std::move(*f.currencies);
    }
    if (f.from) {
        from = *f.from;
    }
    if (f.to) {
        to = *f.to;
    }
    if (f.note) {
        note = *f.note;
    }

    invalidate();
}

bool TransactionProxy::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (!enabled) return true;



    float amt = sourceModel()->index(sourceRow, 0, sourceParent).data(filterRole()).toFloat();
    if (isExpense && ((amt < 0) != *isExpense)) return false;
    if (!qFuzzyIsNull(maxAmount) && amt > maxAmount) return false;

    if (!categories.empty() && !categories.contains(
        sourceModel()->index(sourceRow, 2, sourceParent).data(filterRole()).toString()))
        return false;

    return true;
    QString acc = sourceModel()->index(sourceRow, 3, sourceParent).data(filterRole()).toString();
    if (!accounts.empty() && !accounts.contains(acc)) return false;

    QString cur = sourceModel()->index(sourceRow, 5, sourceParent).data(filterRole()).toString();
    if (!currencies.empty() && !currencies.contains(cur)) return false;

    QDate date  = sourceModel()->index(sourceRow, 1, sourceParent).data(filterRole()).toDate();
    if (!from.isNull() && date < from) return false;
    if (!to.isNull() && date > to) return false;

    QString note = sourceModel()->index(sourceRow, 4, sourceParent).data(filterRole()).toString();
    if (!note.isEmpty() && !note.contains(note)) return false;

    return true;
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
        case 2: return t.categoryName;
        case 3: return t.account;
        case 4: return t.note.isEmpty() ? "None" : t.note;
        }
    }
    else if (role == Qt::UserRole) {
        switch (index.column()) {
        case 0: return t.amount / currencies[t.currency];
        case 1: return t.dateTime;
        case 2: return t.categoryName;

        case 5: return t.currency;
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







