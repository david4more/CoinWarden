#pragma once

#include <QWidget>
class Backend;
class QCustomPlot;
class QCPBars;

namespace Ui { class HomePage; }

class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(Backend* backend, QWidget* parent = nullptr);
    ~HomePage() override;
    void refresh();

private:
    void updateFinancesData();
    void updateCategoriesData();
    void setupFinancesPlot();
    void setupCategoriesPlot();
    QVector<double> smoothGraph(const QVector<double>& data, const QVector<double>& x);

    QCPBars* expenseBar;
    QCPBars* limitBar;
    Backend* backend;
    Ui::HomePage* ui;
};
