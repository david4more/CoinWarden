#pragma once

#include "../Modules/Utils.h"
class QSqlDatabase;
#include "../Modules/Category.h"

class BACKEND_EXPORT CategoriesManager
{
    friend class Backend;
    bool setupDefault();
    bool init();
    QVector<Category> _categories;

    QSqlDatabase& db;
    static const QString defaultColor;

public:

    int findId(QString name, bool isExpense = true) const;
    explicit CategoriesManager(QSqlDatabase& db) : db(db) {}
    QVector<Category> get() const;
    QStringList getNames(CategoryType type = CategoryType::All) const;
    bool add(QString name, bool isExpense, QString color = defaultColor);
};
