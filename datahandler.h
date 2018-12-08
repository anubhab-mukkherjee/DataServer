#ifndef DATAHANDLER_H
#define DATAHANDLER_H
#include <QJsonObject>
#include <QScopedPointer>
#include <QObject>

class DataHandler :public QObject
{
    Q_OBJECT
    static QScopedPointer<DataHandler> s_instance;
    DataHandler();
    static QString filePath;

public:
    QString getDesc();
    bool loginuser(QString username,QString password);
    QString getObject(QString path,QString user);
    void setObject(QString path,QString data,QString user);
    static DataHandler *instance();
    static void setFile(QString filePath);
    virtual ~DataHandler(){}

private:
    QJsonObject jsonObject;

private slots:
    void updateData(const QString &path);

signals:
    void dataChanged();
};


#endif // DATAHANDLER_H
