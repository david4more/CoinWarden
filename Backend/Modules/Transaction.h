#pragma once

#include <QString>
#include <QDateTime>

class Transaction
{
public:
    // recurrence
    float amount;
    QString currency;
    QDateTime dateTime;
    QString category;
    QString account;
    QString note;
    int id;

    explicit operator bool() const {
        return !(amount == 0.f || currency.isEmpty() || dateTime.isNull() || category.isEmpty() || account.isEmpty());
    }
};
