#pragma once
#include <QObject>
class QNetworkRequest;

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    QJsonObject executeNetworkRequest(QNetworkRequest request);
    NetworkManager(QObject* parent = nullptr) : QObject(parent) {}
};
