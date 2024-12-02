#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "Socketinit.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);


    SocketInit *socket = new SocketInit();
    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(QUrl(QStringLiteral("qrc:/qmlFile/Main2.qml")));

    return app.exec();
}
