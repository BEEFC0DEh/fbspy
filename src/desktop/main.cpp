#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "../lib/fbspy.h"
#include "../lib/fbfilespy.h"
#include "../lib/fbviewitem.h"
#include "../lib/fbserver.h"
#include "../lib/fbsocketspy.h"

int main(int argc, char *argv[])
{
    qmlRegisterUncreatableType<FbSpy>("FbSpy", 1, 0, "FbSpy", "FbSpy shouldn't be used directly. Use FbFileSpy or FbSocketSpy insted.");
    qmlRegisterType<FbFileSpy>("FbSpy", 1, 0, "FbFileSpy");
    qmlRegisterType<FbViewItem>("FbSpy", 1, 0, "FbView");
    qmlRegisterType<FbServer>("FbSpy", 1, 0, "FbServer");
    qmlRegisterType<FbSocketSpy>("FbSpy", 1, 0, "FbSocketSpy");

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
