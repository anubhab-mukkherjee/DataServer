#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <QObject>
#include <QDebug>
#include <QTcpServer>


class TCPServer: public QTcpServer
{
    Q_OBJECT
public:
    TCPServer(quint16 port=9080, QObject* parent = nullptr);

public slots:
private:
    //void incomingConnection(qintptr handle) override;
    void newConnection();
protected:
    void incomingConnection(qintptr handle) override;
};

#endif // TCPSERVER_H
