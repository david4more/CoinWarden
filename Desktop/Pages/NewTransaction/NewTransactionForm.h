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

    void clear();

signals:
    void done();

private:
    void onAddTransaction();
    void updateData();

    Backend* backend;
    Ui::NewTransactionForm* ui;
};
