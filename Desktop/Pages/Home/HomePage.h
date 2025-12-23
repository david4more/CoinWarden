#pragma once

#include <QWidget>
#include <QVector>
#include <../Backend/Modules/Utils.h>
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
    void setData(QVector<QPair<QString, double>> t, QMap<QString, double> l, QVector<DailyTransactions> d, QString base);

signals:
    void requestData();

private:
    QCPBars* expenseBar;
    QCPBars* limitBar;
    Backend* backend;
    Ui::HomePage* ui;

    QVector<QPair<QString, double>> transactionsData;
    QMap<QString, double> limitsData;
    QVector<DailyTransactions> dailyData;
    QString baseCurrency;

    void updateFinancesIndicator();
    void updateFinancesData();
    void updateCategoriesData();
    void setupFinancesPlot();
    void setupCategoriesPlot();
    QVector<double> smoothGraph(const QVector<double>& data, const QVector<double>& x);
};
