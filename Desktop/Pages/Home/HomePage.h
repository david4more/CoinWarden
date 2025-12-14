#pragma once

#include <QWidget>
class Backend;
class QCustomPlot;

namespace Ui { class HomePage; }

class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(Backend* backend, QWidget* parent = nullptr);
    ~HomePage() override;

private:
    void setupFinancesPlot(QCustomPlot *customPlot);
    void setupCategoriesPlot(QCustomPlot *customPlot);
    QVector<double> smoothGraph(const QVector<double>& data, const QVector<double>& x);

    Backend* backend;
    Ui::HomePage* ui;
};
