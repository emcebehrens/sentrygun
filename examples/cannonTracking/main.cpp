#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickItem>

#include "cpp_qml_glue.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<CppQmlGlue>("CppQmlGlue", 1, 0, "CppQmlGlue");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    return app.exec();
}
