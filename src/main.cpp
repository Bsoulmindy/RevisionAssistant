#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml/QQmlExtensionPlugin>
#include "../config/config.h"
#include <QFontDatabase>
#include <QIcon>
#include <thread>
#include <chrono>

#if defined(Q_OS_WASM)
#include <emscripten.h>
#endif

QApplication* app = nullptr;
QQmlApplicationEngine* engine = nullptr;
int global_argc = 0;
char** global_argv = nullptr;

// Starting the app
// Here's the primary point to start the application after executing all prerequisites
int init(int argc, char *argv[])
{
    app = new QApplication(argc, argv);

    Q_INIT_RESOURCE(resources);

    engine = new QQmlApplicationEngine();
    QObject::connect(
        engine,
        &QQmlApplicationEngine::objectCreationFailed,
        app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    Q_IMPORT_QML_PLUGIN(CustomComponentsPlugin)

    // Setting up the defaults
    int id = QFontDatabase::addApplicationFont(":/fonts/stratum2demium.ttf");
    if(id == -1) {
        qCritical() << "Couldn't load the font";
    } else {
        QString family_font = QFontDatabase::applicationFontFamilies(id).at(0);
        QFont stratum_font(family_font);
        app->setFont(stratum_font);
    }
    app->setWindowIcon(QIcon(":/icons/revision_assistant_icon.png"));

    QCoreApplication::setOrganizationName("Bsoulmindy");
    QCoreApplication::setApplicationName("Revision Assistant");

    engine->rootContext()->setContextProperty("appVersion", PROJECT_VERSION);
    engine->loadFromModule("RevisionAssistant", "Main");

#if defined(Q_OS_WASM)
    return 0;
#else
    return app->exec();
#endif
}

int main(int argc, char *argv[])
{
    global_argc = argc;
    global_argv = argv;
#if defined(Q_OS_WASM)
    // Prepare Persistent storage for WASM
    EM_ASM(
        if (!FS.analyzePath('/src').exists)
        FS.mkdir('/src');

        FS.mount(IDBFS, {}, '/src');
        FS.syncfs(true, function (err) {
            if(err)
                console.error(err);
            Module._start();
        });
    );

#endif
#if defined(Q_OS_WASM)
    return 0;
#else
    return init(argc, argv);
#endif
}

#if defined(Q_OS_WASM)
extern "C" {
    void EMSCRIPTEN_KEEPALIVE start() {
        init(global_argc, global_argv);
    }
}
#endif
