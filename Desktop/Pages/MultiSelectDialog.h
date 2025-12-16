#pragma once
#include <QDialog>

namespace Ui { class MultiSelectDialog; }

class MultiSelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MultiSelectDialog(QWidget* parent = nullptr);
    ~MultiSelectDialog() override;

private:
    Ui::MultiSelectDialog* ui;
};