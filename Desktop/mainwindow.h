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
class QButtonGroup;
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

    void setupUI();
    void updateUI();
    void onFirstLaunch();
    void changePage(Page p);
};









