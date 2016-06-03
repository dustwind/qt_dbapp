#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <dbmanager.h>
#include <repository.h>
#include <qtextcodec.h>
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QTextCodec *codec1 = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec1);
    //QTextCodec::setCodecForTr(codec1);
    //QTextCodec::setCodecForCStrings(codec1);


    qmlRegisterType<DBManager>("Lab2.DBManager",1,0,"DBManager");
    qmlRegisterType<Repository>("Lab2.Repository",1,0,"Repository");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
