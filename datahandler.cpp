#include "datahandler.h"
#include <QFile>
#include <QJsonDocument>
#include <QTextStream>
#include <QFileSystemWatcher>
#include <QVector>

QScopedPointer<DataHandler> DataHandler::s_instance(nullptr);

DataHandler::DataHandler()
    :  QObject(nullptr)
{
    updateData("../data.json");
    QFileSystemWatcher* watcher = new QFileSystemWatcher(this);
    watcher->addPath("../data.json");
    connect(watcher,&QFileSystemWatcher::fileChanged,this,&DataHandler::updateData);
}

QString DataHandler::getDesc()
{
    return jsonObject["desc"].toString();
}

bool DataHandler::loginuser(QString username, QString password)
{
    if(!jsonObject.contains(username))
    {
        return false;
    }
    if(jsonObject[username].toObject()["pass"].toString()!=password.trimmed())
    {
        return false;
    }
    return true;
}

DataHandler *DataHandler::instance()
{
    if(s_instance==nullptr)
        s_instance.reset(new DataHandler);
    return s_instance.data();
}


void DataHandler::updateData(const QString& path)
{
    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    jsonObject = QJsonDocument::fromJson(file.readAll()).object();
    file.close();
    emit dataChanged();
}

QString DataHandler::getObject(QString path,QString user)
{
    QStringList pathData = path.split(".");
    QString data="";
    QJsonObject j = jsonObject[user].toObject()["data"].toObject();
    for(int i=0; i<pathData.length()-1;i++)
    {
        j = j[pathData[i]].toObject();
    }
    QJsonValue a=j[pathData.last()];
    if(a.isString())
    {
        return a.toString();
    }
    else
    {
        return QJsonDocument(a.toObject()).toJson(QJsonDocument::JsonFormat::Compact);
    }
}

void DataHandler::setObject(QString path, QString data, QString user)
{
    path.prepend(user+".data.");
    QStringList pathData = path.split(".");
    QVector<QJsonObject> v;
    v.push_back(jsonObject[pathData[0]].toObject());
    for(int i=1; i<pathData.length()-1;i++)
    {
        v.push_back(v.last()[pathData[i]].toObject());
    }
    v.last().insert(pathData.last(),data);
    v[v.length()-2].insert(pathData[pathData.length()-2],v[v.length()-1]);

    for(int i=v.length()-2;i>1;i--)
    {
        v[i-1].insert(pathData[i],v[i]);
    }
    jsonObject.insert(user,v[0]);

    QFile file("../data.json");
    file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
    file.write(QJsonDocument(jsonObject).toJson());
    file.close();
    emit dataChanged();
}
