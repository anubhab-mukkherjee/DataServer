#include "clientmanager.h"
#include <QTextStream>
#include <datahandler.h>
#include <QThreadPool>

ClientManager::ClientManager(QObject *parent)
    :QObject(parent)
{
    connect(socket,&QTcpSocket::connected,[&]()
    {
        QTextStream(stdout) << "Client connected " << socket->socketDescriptor() <<"\n\n";
    });
    connect(socket,&QTcpSocket::disconnected,[&]()
    {
        QTextStream(stdout) << "Client disconnected " << socket->socketDescriptor() <<"\n\n";
    });
    connect(socket,&QTcpSocket::readyRead,[&]()
    {/**/

        QThread *thread = QThread::create([&]()
        {
            QString string="";
            while (!string.contains("exit"))
            {
                socket->waitForReadyRead(3000);
                string = socket->readAll();
                QStringList list = string.split(" ");
                if(list[0]=="connect")
                {
                    socket->write((DataHandler::instance()->loginuser(list[1],list[2]))
                            ?
                                "Connection done"
                              :
                                "Check your username or/and password");
                }
            }

            socket->close();

            if(string.contains("shut"))
                exit(0);
        });
        thread->start();
    });
}

void ClientManager::setSocket(QTcpSocket *handle)
{
    //setSocketDescriptor(handle);
    socket = handle;

}
