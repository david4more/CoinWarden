#include "HomePage.h"
#include "ui_HomePage.h"
#include "../QCustomPlot/qcustomplot.h"

#include "../Backend/Backend.h"
#include "Managers/TransactionsManager.h"
#include "Managers/CurrenciesManager.h"
#include "Managers/CategoriesManager.h"
#include <QDateTime>

HomePage::~HomePage() { delete ui; }

void HomePage::updateData()
{
    updateFinancesData();
    updateCategoriesData();
}

HomePage::HomePage(Backend* backend, QWidget* parent) :
    QWidget(parent), ui(new Ui::HomePage), backend(backend)
{
    ui->setupUi(this);

    setupFinancesPlot();
    updateFinancesData();

    setupCategoriesPlot();
    updateCategoriesData();
}

void HomePage::setupCategoriesPlot()
{
    auto* plot = ui->categoriesPlot;

    plot->setBackground(QBrush(QColor(30, 30, 30)));

    expenseBar = new QCPBars(plot->xAxis, plot->yAxis);
    expenseBar->setPen(QPen(QColor(Qt::red).darker(150), 2));
    expenseBar->setBrush(QBrush(QColor(Qt::red)));

    limitBar = new QCPBars(plot->xAxis, plot->yAxis);
    limitBar->setPen(QPen(QColor(Qt::blue).darker(75), 2));
    limitBar->setBrush(QBrush(QColor(Qt::transparent)));

    plot->xAxis->setBasePen(QPen(Qt::white));
    plot->xAxis->setTickPen(QPen(Qt::white));
    plot->xAxis->setTickLabelColor(Qt::white);
    plot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));

    plot->yAxis->setBasePen(QPen(Qt::white));
    plot->yAxis->setTickPen(QPen(Qt::white));
    plot->yAxis->setTickLabelColor(Qt::white);
    plot->yAxis->setLabelColor(Qt::white);
    plot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));

    plot->xAxis->setTickLength(0, 4);
}

void HomePage::updateCategoriesData()
{
    auto* plot = ui->categoriesPlot;

    QVector<double> ticks;
    QVector<double> values, limits;
    QVector<QString> labels;
    auto data = backend->transactions()->transactionsPerCategory(QDate::currentDate().addMonths(-1),QDate::currentDate(), CategoryType::Expense);
    if (data.size() == 0) return;

    auto limitsData = backend->categories()->getLimits();
    for (int i = 0; i < data.size(); ++i) {
        ticks << i + 1;
        values << -data[i].second;

        auto label = data[i].first;
        limits << limitsData[label];
        if (label.size() > 10) label = label.left(5) + ".";
        labels << label;
    }

    expenseBar->setData(ticks, values);
    limitBar->setData(ticks, limits);


    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    plot->xAxis->setTicker(textTicker);
    plot->xAxis->setRange(0, data.size() + 1);

    plot->yAxis->setRange(values[0] * -0.05f, values[0] * 1.05f);
    plot->yAxis->setLabel("Expense per category\nin " + backend->currencies()->base());

    plot->replot();
}

void HomePage::setupFinancesPlot()
{
    auto* plot = ui->financesPlot;

    plot->addGraph();
    plot->graph(0)->setPen(QPen(Qt::blue));
    plot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    plot->addGraph();
    plot->graph(1)->setPen(QPen(Qt::red));
    plot->graph(1)->setBrush(QBrush(QColor(255, 0, 0, 20)));

    plot->xAxis2->setVisible(true);
    plot->xAxis2->setTickLabels(false);
    plot->yAxis2->setVisible(true);
    plot->yAxis2->setTickLabels(false);

    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

void HomePage::updateFinancesData()
{
    auto* plot = ui->financesPlot;

    auto data = backend->transactions()->transactionsPerDay(
        QDate::currentDate().addMonths(-1),
        QDate::currentDate());

    QVector<double> x(data.size()), y0(data.size()), y1(data.size());
    if (data.size() == 0) return;
    for (int i= 0; i < data.size(); ++i)
    {
        x[i] = i;
        y0[i] = data[i].income;
        y1[i] = data[i].expense;
    }

    plot->graph(0)->setData(x, y0);
    plot->graph(1)->setData(x, y1);

    plot->rescaleAxes();
}

QVector<double> HomePage::smoothGraph(const QVector<double>& data, const QVector<double>& x)
{
    if (data.isEmpty() || data.size() > x.size()) return {};

    QVector<double> smoothed(data.size());

    int smoothness = data.size() / 40;
    int half = smoothness / 2;

    for (int i = 0; i < data.size(); ++i) {
        double sum = 0.0;
        int count = 0;

        // accumulate values in a window
        for (int j = -half; j <= half; ++j) {
            int idx = i + j;
            if (idx >= 0 && idx < data.size()) {
                sum += data[idx];
                ++count;
            }
        }

        smoothed[i] = sum / count;
    }

    return smoothed;
}

