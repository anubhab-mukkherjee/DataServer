#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H
#include<QTcpSocket>

class ClientManager : public QObject
{
    Q_OBJECT
    QTcpSocket *socket;
public:
    ClientManager(QObject *parent);
    void setSocket(QTcpSocket *handle);
};

#endif // CLIENTMANAGER_H
