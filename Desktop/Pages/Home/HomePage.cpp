#include "HomePage.h"
#include "ui_HomePage.h"
#include "../QCustomPlot/qcustomplot.h"

#include "../Backend/Backend.h"
#include "../Backend/Modules/TransactionsManager.h"
#include "../Backend/Modules/CurrenciesManager.h"
#include <QDateTime>

HomePage::HomePage(Backend* backend, QWidget* parent) :
    QWidget(parent), ui(new Ui::HomePage), backend(backend)
{
    ui->setupUi(this);

    setupFinancesPlot(ui->financesPlot);
    setupCategoriesPlot(ui->categoriesPlot);
}

void HomePage::setupFinancesPlot(QCustomPlot *plot)
{
    plot->addGraph();
    plot->graph(0)->setPen(QPen(Qt::blue));
    plot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    plot->addGraph();
    plot->graph(1)->setPen(QPen(Qt::red));
    plot->graph(1)->setBrush(QBrush(QColor(255, 0, 0, 20)));

    auto data = backend->transactions()->transactionsPerDay(QDate::currentDate().addMonths(-1), QDate::currentDate());
    QVector<double> x(data.size()), y0(data.size()), y1(data.size());
    if (data.size() == 0) return;
    for (int i= 0; i < data.size(); ++i)
    {
        x[i] = i;
        y0[i] = data[i].income;
        y1[i] = data[i].expense;
    }

    // configure right and top axis to show ticks but no labels:
    // (see QCPAxisRect::setupFullAxesBox for a quicker method to do this)
    plot->xAxis2->setVisible(true);
    plot->xAxis2->setTickLabels(false);
    plot->yAxis2->setVisible(true);
    plot->yAxis2->setTickLabels(false);
    plot->graph(0)->setData(x, y0);
    plot->graph(1)->setData(x, y1);

    plot->rescaleAxes();
    // Allow the user to drag axis ranges with the mouse, zoom with the mouse wheel and select graphs by clicking:
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

void HomePage::setupCategoriesPlot(QCustomPlot *plot)
{
    plot->setBackground(QBrush(QColor(30, 30, 30)));

    QCPBars *expenseBar = new QCPBars(plot->xAxis, plot->yAxis);
    expenseBar->setPen(QPen(QColor(Qt::red).darker(150), 2));
    expenseBar->setBrush(QBrush(QColor(Qt::red)));

    plot->xAxis->setBasePen(QPen(Qt::white));
    plot->xAxis->setTickPen(QPen(Qt::white));
    plot->xAxis->setTickLabelColor(Qt::white);
    plot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));

    plot->yAxis->setBasePen(QPen(Qt::white));
    plot->yAxis->setTickPen(QPen(Qt::white));
    plot->yAxis->setTickLabelColor(Qt::white);
    plot->yAxis->setLabelColor(Qt::white);
    plot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));

    QVector<double> ticks;
    QVector<double> values;
    QVector<QString> labels;
    auto data = backend->transactions()->transactionsPerCategory(
        QDate::currentDate(),
        QDate::currentDate().addMonths(-1),
        CategoryType::Expense);

    if (data.size() == 0) return;
    else
        for (int i = 0; i < data.size(); ++i) {
            ticks << i + 1;
            labels << data[i].first;
            values << -data[i].second;
        }

    expenseBar->setData(ticks, values);

    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    plot->xAxis->setTicker(textTicker);
    plot->xAxis->setTickLength(0, 4);
    plot->xAxis->setRange(0, data.size() + 1);

    plot->yAxis->setRange(values[0] * -0.05f, values[0] * 1.05f);
    plot->yAxis->setLabel("Expense per category\nin " + backend->currencies()->base());

    plot->replot();
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

HomePage::~HomePage()
{
    delete ui;
}
