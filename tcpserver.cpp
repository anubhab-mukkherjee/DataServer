#include "tcpserver.h"
#include <QTcpSocket>
#include <datahandler.h>
#include <QThread>
#include <QThreadPool>
#include <threadedsocket.h>
#include <QCoreApplication>
#include <QCommandLineParser>

TCPServer::TCPServer(quint16 port,QObject *parent)
    :QTcpServer (parent)
{
    //connect(this,&QTcpServer::newConnection,this,&TCPServer::newConnection);
    if(!listen(QHostAddress::Any,port))
    {
        qDebug() << "Server could not start on port"<< port;
    }
    else
    {
        qDebug() << "Server started on port"<< port;
    }
}
void TCPServer::incomingConnection(qintptr handle)
{
    //qDebug() << Q_FUNC_INFO << " new connection";
    ThreadedSocket *socket = new ThreadedSocket(handle);
    connect(socket, &ThreadedSocket::finished, socket, &ThreadedSocket::deleteLater);
    connect(socket, &ThreadedSocket::closeServer, this , []()
    {
        exit(0);
    });
    socket->start();
}
