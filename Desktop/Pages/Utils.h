#pragma once
#include <QObject>
#include <QTimer>

enum class Page { Home, Transactions, Settings, NewTransaction, CustomFilters };


class Utils : public QObject
{
    Q_OBJECT

public:
    static void highlightField(QWidget* widget, bool condition = false)
    {
        if (!condition) return;

        auto* timer = widget->findChild<QTimer*>("errorTimer");
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
};