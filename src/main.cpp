#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml/QQmlExtensionPlugin>
#include "../config/config.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Q_INIT_RESOURCE(resources);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    Q_IMPORT_QML_PLUGIN(CustomComponentsPlugin)
    engine.rootContext()->setContextProperty("appVersion", PROJECT_VERSION);
    engine.loadFromModule("RevisionAssistant", "Main");

    return app.exec();
}
