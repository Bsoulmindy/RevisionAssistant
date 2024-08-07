#include <QObject>
#include <QTest>
#include "tst_setup.h"
#include "testgroup/tst_dict_repo_factory.h"
#include "testgroup/tst_dict_json_repo.h"
#include "testgroup/tst_filesystemutils.h"

class TestSuiteMain: public QObject
{
    Q_OBJECT

public:
    TestSuiteMain() {
        m_test_groups.push_back(std::make_unique<DictRepoFactoryTest>());
        m_test_groups.push_back(std::make_unique<DictJSONRepoTest>());
        m_test_groups.push_back(std::make_unique<FileSystemUtilsTest>());
    }

private:
    Setup m_setup;
    std::vector<std::unique_ptr<ITestGroup>> m_test_groups;

private slots:
    void init()
    {
        m_setup.global_preparation();
    }

    void dictRepoFactoryTest()
    {
        try {
            m_test_groups.at(0)->run_tests();
        } catch (std::exception& e) {
            QFAIL(e.what());
        }
    }

    void dictJsonRepoTest()
    {
        try {
            m_test_groups.at(1)->run_tests();
        } catch (std::exception& e) {
            QFAIL(e.what());
        }
    }

    void fileSystemUtilsTest()
    {
        try {
            m_test_groups.at(2)->run_tests();
        } catch (std::exception& e) {
            QFAIL(e.what());
        }
    }

    void cleanup()
    {
        m_setup.global_cleanup();
    }
};

QTEST_MAIN(TestSuiteMain)
#include "tst_main.moc"
