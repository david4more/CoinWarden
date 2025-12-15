#pragma once

#include <QDialog>
class Backend;

namespace Ui { class CustomFiltersForm; }

class CustomFiltersForm : public QDialog
{
    Q_OBJECT

public:
    explicit CustomFiltersForm(Backend* backend, QWidget* parent = nullptr);
    ~CustomFiltersForm() override;

    void updateData();

private:
    Backend* backend;
    Ui::CustomFiltersForm* ui;

    void clearForm();
    void onAddCategory();
    void onApplyCustomFilters();
    void setupButtonGroups();
};
