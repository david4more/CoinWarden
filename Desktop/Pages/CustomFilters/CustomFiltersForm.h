#pragma once

#include <QDialog>
class Backend;
class TransactionProxy;

namespace Ui { class CustomFiltersForm; }

class CustomFiltersForm : public QDialog
{
    Q_OBJECT

public:
    explicit CustomFiltersForm(Backend* backend, TransactionProxy* proxy, QWidget* parent = nullptr);
    ~CustomFiltersForm() override;

    void updateData();

private:
    Backend* backend;
    Ui::CustomFiltersForm* ui;
    TransactionProxy* proxy;

    void onButtonGroupClicked(int index);
    void onApplyCustomFilters();
};
