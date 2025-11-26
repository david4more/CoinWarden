#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Modules/transaction.h"
#include "Modules/currency.h"
#include "Modules/category.h"
#include "Modules/account.h"
#include "../Backend/backend.h"
#include "../Backend/model.h"
#include "../Backend/Modules/c.h"
#include "qcustomplot.h"
#include <QTimer>
#include <QButtonGroup>
#include <QStyledItemDelegate>
#include <QDialog>

class TransactionDelegate : public QStyledItemDelegate
{
public:
    explicit TransactionDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        QStyleOptionViewItem opt(option);
        opt.state &= ~QStyle::State_MouseOver;
        QStyledItemDelegate::paint(painter, opt, index); }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupUI();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete model;
    delete proxy;
    delete backend;
}

void MainWindow::connectSlots()
{
    connect(ui->homeButton, &QToolButton::clicked, this, [this]{ changePage(Page::home); });
    connect(ui->transactionsButton, &QToolButton::clicked, this, [this]{ changePage(Page::transactions); });
    connect(ui->settingsButton, &QToolButton::clicked, this, [this]{ changePage(Page::settings); });
    connect(ui->newTransactionButton, &QPushButton::clicked, this, [this]{ changePage(Page::newTransaction);});
    connect(ui->addTransactionButton, &QPushButton::clicked, this, &MainWindow::onAddTransaction);
    connect(ui->applyCustomFiltersButton, &QPushButton::clicked, this, &MainWindow::onApplyCustomFilters);

    connect(ui->prevMonthButton, &QToolButton::clicked, this, [&]{      onMonthButton(false); });
    connect(ui->nextMonthButton, &QToolButton::clicked, this, [&]{      onMonthButton(true); });
    connect(ui->noFilterButton, &QToolButton::clicked, this, [&]{       proxy->useFilters({.enabled = false}); });
    connect(ui->expenseFilterButton, &QToolButton::clicked, this, [&]{  proxy->useFilters({.maxAmount = 0.f}); });
    connect(ui->incomeFilterButton, &QToolButton::clicked, this, [&]{   proxy->useFilters({.minAmount = 0.f}); });
    connect(ui->categoryFilterButton, &QToolButton::clicked, this, &MainWindow::onCategoryFilterButton);
    connect(ui->customFilterButton, &QToolButton::clicked, this, [this]{ changePage(Page::customFilters);});
    connect(ui->dateButton, &QToolButton::clicked, this, [this]{ changePage(Page::customFilters);});

    connect(ui->tExpense, &QToolButton::clicked, this, [this]{  });
}

void MainWindow::setupTransactionsTable()
{
    ui->transactionsTable->resizeColumnsToContents();
    ui->transactionsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->transactionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->transactionsTable->setAlternatingRowColors(true);
    ui->transactionsTable->setSortingEnabled(true);
    ui->transactionsTable->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->transactionsTable->setItemDelegate(new TransactionDelegate(ui->centralwidget));
}

void MainWindow::setupButtonGroups()
{
    QButtonGroup* group = new QButtonGroup(this);
    group->setExclusive(true);
    group->addButton(ui->noFilterButton);
    group->addButton(ui->expenseFilterButton);
    group->addButton(ui->incomeFilterButton);
    group->addButton(ui->categoryFilterButton);
    group->addButton(ui->customFilterButton);

    group = new QButtonGroup(this);
    group->setExclusive(true);
    group->addButton(ui->tExpense);
    group->addButton(ui->tIncome);
}

void MainWindow::setupUI()
{
    backend = new Backend();
    // connect(backend, &Backend::firstLaunch, this, &MainWindow::onFirstLaunch);
    backend->init();

    model = new TransactionModel(ui->centralwidget);
    proxy = new TransactionProxy(ui->centralwidget);
    proxy->setSourceModel(model);
    ui->transactionsTable->setModel(proxy);

    connectSlots();
    setupTransactionsTable();
    setupButtonGroups();

    changePage(Page::home);
    from = QDate(QDate::currentDate().year(), QDate::currentDate().month(), 1);
    to = QDate(QDate::currentDate().year(), QDate::currentDate().month(), QDate::currentDate().daysInMonth());
    updateTransactions();
    updateData();

    setupFinancesPlot(ui->financesPlot);
    setupCategoriesPlot(ui->categoriesPlot);
}

void MainWindow::setupFinancesPlot(QCustomPlot *plot)
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
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

QVector<double> MainWindow::smoothGraph(const QVector<double>& data, const QVector<double>& x)
{
    if (data.isEmpty() || data.size() > x.size()) return {};

    QVector<double> smoothed(data.size());

    int smoothness = data.size() / 40;
    int half = smoothness / 2;

    for (int i = 0; i < data.size(); ++i) {
        double sum = 0.0;
        int count = 0;

        // accumulate values in window
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

void MainWindow::onMonthButton(bool next)
{
    from = from.addMonths(next ? 1 : -1);
    to = QDate(from.year(), from.month(), from.daysInMonth());

    updateTransactions();
}

void MainWindow::onAddCategory()
{
    ;
}

void MainWindow::onApplyCustomFilters()
{
    ;
}

void MainWindow::updateTransactions()
{
    ui->dateButton->setText(from.toString("MMMM yyyy"));
    model->setTransactions(backend->transactions()->get(from, to));
}

void MainWindow::updateData()
{
    auto updateCombo = [&](QComboBox* box, QStringList list) {
        box->clear();
        for (const auto& x : list) box->addItem(x);
    };
    updateCombo(ui->tCurrency, backend->currencies()->codes());
    updateCombo(ui->tAccount, backend->accounts()->names());
    updateCombo(ui->tCategory, backend->categories()->getNames(CategoryType::Expense));

    updateCombo(ui->fExpenseCategories, backend->categories()->getNames(CategoryType::Expense));
    updateCombo(ui->fIncomeCategories, backend->categories()->getNames(CategoryType::Income));
}

void MainWindow::setupCategoriesPlot(QCustomPlot *plot)
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
    auto data = backend->transactions()->transactionsPerCategory(from, to, CategoryType::Expense);

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
    plot->yAxis->setLabel("Expense per category\nin " + backend->currencies()->def());

    plot->replot();
}

void MainWindow::onCategoryFilterButton()
{
    QDialog* dialog = new QDialog(this);
    QVBoxLayout* layout = new QVBoxLayout(dialog);

    for (const auto& item : backend->categories()->getNames()) {
        QCheckBox* box = new QCheckBox(item, dialog);
        box->setChecked(pickedCategories.contains(item));
        layout->addWidget(box);
    }

    QPushButton* button = new QPushButton("Done", dialog);
    layout->addWidget(button);

    // Update pickedCategories whenever a checkbox is clicked
    auto updateCategoriesFilter = [dialog, this]{
        pickedCategories.clear();
        for (auto* box : dialog->findChildren<QCheckBox*>()) {
            if (box->checkState() == Qt::Checked)
                pickedCategories.append(box->text());
        }

        proxy->useFilters({.categories = pickedCategories});
    };

    for (auto* box : dialog->findChildren<QCheckBox*>())
        connect(box, &QCheckBox::clicked, dialog, updateCategoriesFilter);

    connect(button, &QPushButton::clicked, dialog, [dialog]{ dialog->accept(); });

    connect(dialog, &QDialog::finished, this, [=](int result) {
        if ((result == QDialog::Accepted && pickedCategories.empty()) || result == QDialog::Rejected)
            ui->noFilterButton->click();
    });

    updateCategoriesFilter();
    dialog->setWindowModality(Qt::WindowModal);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

void MainWindow::onAddTransaction()
{
    Transaction t;
    t.amount = (ui->tIncome->isChecked()) ? ui->tAmount->value() : -ui->tAmount->value();
    t.currency = ui->tCurrency->currentText();
    t.dateTime = QDateTime(ui->tDate->date(), ui->tTime->time());
    t.category = ui->tCategory->currentText();
    t.account = ui->tAccount->currentText();
    t.note = ui->tNote->text();

    if (!t) {
        highlightField(ui->tAmount, ui->tAmount->value() == 0.f);
        return;
    }

    backend->transactions()->add(std::move(t));

    updateTransactions();
    changePage(Page::transactions);
}

void MainWindow::highlightField(QWidget* widget, bool condition)
{
    if (!condition) return;

    QTimer* timer = widget->findChild<QTimer*>("errorTimer");
    if (!timer) {
        timer = new QTimer(widget);
        timer->setObjectName("errorTimer");
        timer->setSingleShot(true);
        connect(timer, &QTimer::timeout, [widget]() {
            if (widget) widget->setStyleSheet("");
        });
    }

    widget->setStyleSheet("background-color: #5a1f1f;");
    timer->stop();
    timer->start(2000);
}

void MainWindow::clearTransactionForm()
{
    ui->tAmount->setValue(0);
    ui->tCurrency->setCurrentIndex(0);
    ui->tDate->setDate(QDate::currentDate());
    ui->tTime->setTime(QTime::currentTime());
    ui->tCategory->setCurrentIndex(0);
    ui->tAccount->setCurrentIndex(0);
    ui->tNote->clear();
}

void MainWindow::changePage(Page p)
{
    ui->pages->setCurrentIndex(p);

    if (p == Page::newTransaction) clearTransactionForm();
}

void MainWindow::onFirstLaunch()
{
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

    backend->transactions()->setupDefault();
    backend->categories()->setupDefault(); // dialog->exec();
}





















