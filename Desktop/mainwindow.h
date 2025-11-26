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
    static QVector<double> smoothGraph(const QVector<double>& data, const QVector<double>& x);
    void updateTransactions() const;
    void updateData() const;
    void setupFinancesPlot(QCustomPlot *customPlot) const;
    void setupCategoriesPlot(QCustomPlot *customPlot) const;

    void changePage(Page p);
    static void highlightField(QWidget* widget, bool condition);
    void clearTransactionForm();

    void connectSlots();
    void setupTransactionsTable() const;
    void setupButtonGroups();
};









