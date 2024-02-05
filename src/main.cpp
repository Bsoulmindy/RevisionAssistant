#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml/QQmlExtensionPlugin>
#include "../config/config.h"
#include <QFontDatabase>
#include <QIcon>

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

    int id = QFontDatabase::addApplicationFont(":/fonts/stratum2demium.ttf");
    if(id == -1) {
        qCritical() << "Couldn't load the font";
    } else {
        QString family_font = QFontDatabase::applicationFontFamilies(id).at(0);
        QFont stratum_font(family_font);
        app.setFont(stratum_font);
    }
    app.setWindowIcon(QIcon(":/icons/revision_assistant_icon.png"));

    engine.rootContext()->setContextProperty("appVersion", PROJECT_VERSION);
    engine.loadFromModule("RevisionAssistant", "Main");

    return app.exec();
}