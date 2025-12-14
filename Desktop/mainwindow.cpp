#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../Backend/Backend.h"
#include "Pages/Home/HomePage.h"
#include "Pages/Transactions/TransactionsPage.h"
#include "Pages/Settings/SettingsPage.h"
#include "Pages/NewTransaction/NewTransactionForm.h"
#include "Pages/CustomFilters/CustomFiltersForm.h"

#include <QButtonGroup>

MainWindow::~MainWindow() { delete ui; }
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupUI();
}

void MainWindow::setupUI()
{
    backend = new Backend(this);
    connect(backend, &Backend::firstLaunch, this, &MainWindow::onFirstLaunch);
    backend->initialize();

    ui->pages->addWidget(homePage = new HomePage(backend, ui->pages));
    ui->pages->addWidget(transactionsPage = new TransactionsPage(backend, ui->pages));
    ui->pages->addWidget(settingsPage = new SettingsPage(backend, ui->pages));
    ui->pages->addWidget(newTransactionForm = new NewTransactionForm(backend, ui->pages));
    ui->pages->addWidget(customFiltersForm = new CustomFiltersForm(backend, ui->pages));

    connect(ui->home, &QToolButton::clicked, this, [this]{ changePage(Page::Home); });
    connect(ui->transactions, &QToolButton::clicked, this, [this]{ changePage(Page::Transactions); });
    connect(ui->settings, &QToolButton::clicked, this, [this]{ changePage(Page::Settings); });
    connect(transactionsPage, &TransactionsPage::newTransaction, this, [this]{ changePage(Page::NewTransaction); });
    connect(transactionsPage, &TransactionsPage::customFilters, this, [this] { changePage(Page::CustomFilters); });

    connect(newTransactionForm, &NewTransactionForm::done, this, [this]()
        { updateUI(); changePage(Page::Transactions); } );
    connect(settingsPage, &SettingsPage::updateUI, this, [this](){ updateUI(); });

    pages = new QButtonGroup(this);
    pages->addButton(ui->home);
    pages->addButton(ui->transactions);
    pages->addButton(ui->settings);
    pages->setExclusive(true);

    changePage(Page::Home);
}

void MainWindow::updateUI()
{
    transactionsPage->updateTransactions();
    homePage->updateData();
}

void MainWindow::changePage(Page p)
{
    switch (p) {
        case Page::NewTransaction:
        newTransactionForm->clear(); break;
    }

    ui->pages->setCurrentIndex(p);
}

void MainWindow::onFirstLaunch()
{
    /*
    QDialog* dialog = new QDialog(this);
    QVBoxLayout* layout = new QVBoxLayout(dialog);
    QLabel* label = new QLabel("Welcome! Add a few categories to begin:", dialog);
    QVector<QLineEdit*> lines;
    lines.push_back(new QLineEdit(dialog));
    QPushButton* add = new QPushButton("Add", dialog);
    QPushButton* def = new QPushButton("Use default", dialog);
    QPushButton* done = new QPushButton("Done", dialog);
    layout->addWidget(label);
    layout->addWidget(lines.back());
    layout->addWidget(add);
    layout->addWidget(def);
    layout->addWidget(done);

    connect(add, &QPushButton::clicked, dialog, [&]{ lines.push_back(new QLineEdit(dialog)); layout->insertWidget(lines.size() - 1, lines.back()); });
    connect(done, &QPushButton::clicked, dialog, [&]{
        QStringList cats;
        for (auto l : lines)
            if (!l->text().isEmpty())
                cats << l->text();

        if (cats.isEmpty()) {
            label->setText("Unable to proceed without categories");
            return;
        }

        for (auto c : cats) backend->categories()->add(c, true);

        dialog->accept();
    });
    connect(def, &QPushButton::clicked, dialog, [&]{ backend->categories()->setupDefault(); dialog->accept(); });

    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setWindowFlag(Qt::WindowCloseButtonHint, false);
    dialog->exec();
    */

    backend->setupDefault();
}






















