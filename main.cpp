#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QStandardPaths>
#include <QFile>
#include <QDir>
#include <QSqlDatabase>
#include <QQmlContext>
#include "config/config.h"

void createDatabaseCopy()
{
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString databasePath = dataDir + "/dict.sqlite";

    if (!QFile::exists(databasePath))
    {
        if (!QDir().mkpath(dataDir)) {
            qDebug() << "Error: Failed to create directory " << dataDir;
            exit(20001);
            return;
        }
        if (!QFile::copy(":/db/dict.sqlite", databasePath)) {
            qDebug() << "Error: Failed to copy database file";
            exit(20002);
            return;
        }

        if(!QFile::setPermissions(databasePath, QFileDevice::WriteUser | QFileDevice::ReadUser)) {
            qDebug() << "Error: Failed to change permissions to Write";
            exit(20003);
            return;
        }

    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "revision_app");
    db.setDatabaseName(databasePath);

    if (!db.open()) {
        qDebug() << "Error: Failed to create database in " << databasePath;
    }
}


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Q_INIT_RESOURCE(resources);
    createDatabaseCopy();

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.rootContext()->setContextProperty("appVersion", PROJECT_VERSION);
    engine.loadFromModule("RevisionAssistant", "Main");

    return app.exec();
}
