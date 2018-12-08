#include "threadedsocket.h"
#include <QTcpSocket>
#include <QTextStream>
#include <datahandler.h>
ThreadedSocket::ThreadedSocket(qintptr handle, QObject *parent)
    :QThread (parent), m_descriptor(handle) , user("")
{

}

ThreadedSocket::~ThreadedSocket()
{

}

void ThreadedSocket::run()
{
    qDebug() << "Function triggered";
    m_socket = new QTcpSocket();
    m_socket->setSocketDescriptor(m_descriptor);

    connect(m_socket,
            &QTcpSocket::readyRead,
            this,
            &ThreadedSocket::onReadyRead,
            Qt::DirectConnection);
    connect(m_socket,
            &QTcpSocket::disconnected,
            this,
            &ThreadedSocket::onDisconnected,
            Qt::DirectConnection);
    connect(DataHandler::instance(),
            &DataHandler::dataChanged,
            this,
            &ThreadedSocket::onDataChanged,
            Qt::DirectConnection);

    exec();
}

void ThreadedSocket::onConnected()
{
    QTextStream(m_socket) << DataHandler::instance()->getDesc();
}

void ThreadedSocket::onReadyRead()
{
    QTextStream pool(m_socket);
    QString string = "";
    string = pool.readAll();
    qDebug() <<string;
    QStringList list = string.split(" ");
    if(list[0]=="connect")
    {
        if(DataHandler::instance()->loginuser(list[1],list[2]))
        {
            user = list[1];
            password = list[2];
            pool<<"1";
        }
        else
        {
            pool<<"0";
            user=password="";
        }
    }
    else if (list[0]=="set")
    {
        if(user=="")
        {
            pool<<"0";
        }
        else
        {
            DataHandler::instance()->setObject(list[1].trimmed(),list[2],user);
            pool<<"1";
        }
    }
    else if (list[0]=="get")
    {
        pool<<DataHandler::instance()->getObject(list[1].trimmed(),
                user)
                .toStdString()
                .c_str();
    }
    else if (string.contains("shut"))
    {
        m_socket->disconnectFromHost();
        emit closeServer();
        exit(0);
    }
    else if (string.contains("done"))
    {
        m_socket->disconnectFromHost();
        return;
    }
    else if (list[0]=="disconnect")
    {
        user="";
    }
    else
    {
        pool<<"0";
    }
}

void ThreadedSocket::onDisconnected()
{
    m_socket->close();
    quit();
}

void ThreadedSocket::onDataChanged()
{
    if(!DataHandler::instance()->loginuser(user,password))
    {
        user=password="";
        QTextStream(m_socket) << "L";
    }
}
