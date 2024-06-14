#include <QtQuickTest>
#include <QQmlEngine>
#include <QQmlContext>
#include <QGuiApplication>
#include "../src/factories/dict_repo_factory.h"
#include "../src/models/dict_repo_enum.h"
#include "../src/utils/filesystemutils.h"
#include "tst_setup.h"

class SetupQML : public QObject
{
    Q_OBJECT

public:
    SetupQML() {}

private:
    Setup m_setup;

private slots:
    void applicationAvailable()
    {
        m_setup.global_preparation();
    }

    void qmlEngineAvailable(QQmlEngine *engine)
    {
    }

    void cleanupTestCase()
    {
        m_setup.global_cleanup();
    }
};

QUICK_TEST_MAIN_WITH_SETUP(unit_tests, SetupQML)

#include "tst_qml_main.moc"
