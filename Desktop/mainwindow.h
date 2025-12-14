#pragma once

#include <QMainWindow>
#include <QDate>
#include "Pages/Utils.h"

namespace Ui { class MainWindow; }

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

    Ui::MainWindow *ui;
    Backend* backend;

    void setupUI();

    // slots
    void onFirstLaunch();

    // helpers
    void changePage(Page p);

    void setupPages();
    void connectSlots();
};









