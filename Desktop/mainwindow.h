#pragma once

#include <QMainWindow>
#include <QDate>
#include "Pages/Utils.h"

namespace Ui { class MainWindow; }

class HomePage;
class TransactionsPage;
class SettingsPage;
class CustomFiltersForm;
class NewTransactionForm;
class Backend;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Backend* backend;

    HomePage* homePage;
    TransactionsPage* transactionsPage;
    SettingsPage* settingsPage;
    CustomFiltersForm* customFiltersForm;
    NewTransactionForm* newTransactionForm;

    void setupUI();
    void onFirstLaunch();
    void changePage(Page p);
};









