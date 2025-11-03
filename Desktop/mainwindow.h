#pragma once

#include <QMainWindow>
#include <QDate>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class TransactionProxy;
class TransactionModel;
class QCustomPlot;
class Backend;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    enum Page { home, transactions, settings, newTransaction, customFilters };

    Ui::MainWindow *ui;
    Backend* backend;
    TransactionModel* model;
    TransactionProxy* proxy;
    QStringList pickedCategories;
    QDate from, to;

    void setupUI();

    // slots
    void onMonthButton(bool next);
    void onAddTransaction();
    void onApplyCustomFilters();
    void onCategoryFilterButton();
    void onAddCategory();
    void onFirstLaunch();

    // helpers
    QVector<double> smoothGraph(const QVector<double>& x, const QVector<double>& data);
    void updateTransactions();
    void updateData();
    void setupFinancesPlot(QCustomPlot *customPlot);
    void setupCategoriesPlot(QCustomPlot *customPlot);

    void changePage(Page p);
    void highlightField(QWidget* widget, bool condition);
    void clearTransactionForm();
};
