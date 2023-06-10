#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "instrumentcluster.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    InstrumentCluster Cluster;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("Work", &Cluster);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
