#pragma once

#include <QWidget>
class Backend;

namespace Ui { class NewTransactionForm; }

class NewTransactionForm : public QWidget
{
    Q_OBJECT

public:
    explicit NewTransactionForm(Backend* backend, QWidget* parent = nullptr);
    ~NewTransactionForm() override;

private:
    void onAddTransaction();
    static void highlightField(QWidget* widget, bool condition);
    void clearTransactionForm();
    void updateData();

    Backend* backend;
    Ui::NewTransactionForm* ui;
};
