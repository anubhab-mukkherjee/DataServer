#ifndef THREADEDSOCKET_H
#define THREADEDSOCKET_H
#include <QThread>

class QTcpSocket;
class ThreadedSocket : public QThread
{
    Q_OBJECT
public:
    ThreadedSocket(qintptr handle, QObject *parent =nullptr);
    ~ThreadedSocket() override;

protected:
    void run() override;

private slots:
    void onConnected();
    void onReadyRead();
    void onDisconnected();
    void onDataChanged();

private:
    QTcpSocket *m_socket;
    qintptr m_descriptor;
    QString user;
    QString password;

signals:
    void closeServer();

};

#endif // THREADEDSOCKET_H
