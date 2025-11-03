#pragma once

#include <QString>

class Transaction
{
public:
    // recurrence
    float amount;
    QString currency;
    QString dateTime;
    QString category;
    QString account;
    QString note;
    int id;

    explicit operator bool() const {
        return !(amount == 0.f || currency.isEmpty() || dateTime.isEmpty() || category.isEmpty() || account.isEmpty());
    }
};
