#pragma once

#include <QMainWindow>
#include <QDate>

#include "Pages/Utils.h"

namespace Ui { class MainWindow; }

class HomePage;
class TransactionsPage;
class SettingsPage;
class NewTransactionForm;
class Backend;
class QButtonGroup;
class TransactionModel;
class TransactionProxy;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Backend* backend;

    QButtonGroup* pages;
    HomePage* homePage;
    TransactionsPage* transactionsPage;
    SettingsPage* settingsPage;
    NewTransactionForm* newTransactionForm;

    TransactionModel* model;
    TransactionProxy* proxy;

    void setupUI();
    void updateUI();
    void onFirstLaunch();
    void changePage(Page p);
};









