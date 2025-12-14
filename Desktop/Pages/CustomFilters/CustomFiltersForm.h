#pragma once

#include <QWidget>
class Backend;

namespace Ui { class CustomFiltersForm; }

class CustomFiltersForm : public QWidget
{
    Q_OBJECT

public:
    explicit CustomFiltersForm(Backend* backend, QWidget* parent = nullptr);
    ~CustomFiltersForm() override;

private:
    Backend* backend;
    Ui::CustomFiltersForm* ui;

    void onAddCategory();
    void onApplyCustomFilters();
    void setupButtonGroups();
    void updateData();
};
