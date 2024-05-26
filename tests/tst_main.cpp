#include <QtQuickTest>
#include <QQmlEngine>
#include <QQmlContext>
#include <QGuiApplication>
#include "../src/factories/dict_repo_factory.h"
#include "../src/models/dict_repo_enum.h"
#include "../src/utils/filesystemutils.h"

class Setup : public QObject
{
    Q_OBJECT

public:
    Setup() {}

public slots:
    void applicationAvailable()
    {
        ///////////////////////////// Preparing dict files for testing /////////////////////////////
        // "test_empty_dict" : empty Dict
        auto empty_dict_repo = DictRepoFactory::create_dict_repo("test_empty_dict", DictRepoEnum::Json);
        empty_dict_repo->delete_all();

        // "test_basic_dict" : Dict with 3 entries : question #? | response #?
        auto basic_dict_repo = DictRepoFactory::create_dict_repo("test_basic_dict", DictRepoEnum::Json);
        basic_dict_repo->delete_all();
        std::list<QuestionResponseEntry> basic_list;
        for(int i = 1; i <= 10; i++) {
            basic_list.emplace_back(i, "question #" + QString::number(i), "response #" + QString::number(i), false, false);
        }
        basic_dict_repo->insert_multiple_entries(basic_list);

        // "test_large_dict" : Dict with 10000 entries : question #? | response #?
        auto large_dict_repo = DictRepoFactory::create_dict_repo("test_large_dict", DictRepoEnum::Json);
        large_dict_repo->delete_all();
        std::list<QuestionResponseEntry> large_list;
        for(int i = 1; i <= 10000; i++) {
            large_list.emplace_back(i, "question #" + QString::number(i), "response #" + QString::number(i), false, false);
        }
        large_dict_repo->insert_multiple_entries(large_list);
        ////////////////////////////////////////////////////////////////////////////////////////////
    }

    void qmlEngineAvailable(QQmlEngine *engine)
    {
    }

    void cleanupTestCase()
    {
        FileSystemUtils::remove_file("test_empty_dict.json");
        FileSystemUtils::remove_file("test_basic_dict.json");
        FileSystemUtils::remove_file("test_large_dict.json");
    }
};

QUICK_TEST_MAIN_WITH_SETUP(unit_tests, Setup)

#include "tst_main.moc"
