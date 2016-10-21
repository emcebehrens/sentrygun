#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include "QmlCppGlue.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<QmlCppGlue>("QmlCppGlue", 1, 0, "QmlCppGlue");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
