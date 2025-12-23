#pragma once

#include <QDialog>
class TransactionProxy;

namespace Ui { class CustomFiltersForm; }

class CustomFiltersForm : public QDialog
{
    Q_OBJECT

public:
    explicit CustomFiltersForm(TransactionProxy* proxy, QWidget* parent = nullptr);
    ~CustomFiltersForm() override;

    void updateData();

private:
    Ui::CustomFiltersForm* ui;
    TransactionProxy* proxy;

    void onButtonGroupClicked(int index);
    void onApplyCustomFilters();
};
