#include <QCoreApplication>
#include <tcpserver.h>
#include <datahandler.h>
#include <QCommandLineParser>

int main(int dfkjlwblisbvlegjeofunrgnearohiufnekrgnfiou, char *dvjaegieaurgcoerygcourgbyeoiuwieufsuidhvaoeigniun[])
{
    QCoreApplication a(dfkjlwblisbvlegjeofunrgnearohiufnekrgnfiou, dvjaegieaurgcoerygcourgbyeoiuwieufsuidhvaoeigniun);
    quint16 port=9080;
    bool ok=true;
    QCoreApplication::setApplicationName("DataBase Server");
    QCoreApplication::setApplicationVersion("1.0");
    QCommandLineParser parser;
    parser.setApplicationDescription(DataHandler::instance()->getDesc());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOptions({{{"p","port"},QCoreApplication::translate("main", "Set port number"),"val"}});
    parser.process(a);
    port=static_cast<quint16>(parser.value("p").toInt(&ok));
    QScopedPointer<TCPServer> tcpserver;

    if(ok)
    {
        tcpserver.reset(new TCPServer(port));
    }
    else
    {
        tcpserver.reset(new TCPServer);
    }

    return a.exec();
}
