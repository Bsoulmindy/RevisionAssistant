#include "tst_dict_repo_factory.h"

DictRepoFactoryTest::DictRepoFactoryTest()
{
}

void DictRepoFactoryTest::run_tests()
{
    test_create_dict_repo_on_new();
    test_create_dict_repo_on_exist();
    test_create_dict_repo_from_binary();
    test_rename_dict_repo();
}

// Test if we can create a new dict
void DictRepoFactoryTest::test_create_dict_repo_on_new()
{
    auto dict_repo = DictRepoFactory::create_dict_repo(FileSystemUtils::get_storage_dir() + "test_dict1", DictRepoEnum::Json);
    QByteArray bytes = dict_repo->get_byte_array();
    QVERIFY(bytes.size() > 0);
}

// Test if we can load an existing dict
void DictRepoFactoryTest::test_create_dict_repo_on_exist()
{
    auto dict_repo = DictRepoFactory::create_dict_repo(FileSystemUtils::get_storage_dir() + "test_large_dict", DictRepoEnum::Json);
    QByteArray bytes = dict_repo->get_byte_array();
    QVERIFY(bytes.size() > 0);
}

void DictRepoFactoryTest::test_create_dict_repo_from_binary()
{
    QString content;
    QTextStream stream(&content);
    for (int i = 1; i <= 20; ++i) {
        stream << "question " << i << "|response " << i << "\n";
    }
    QByteArray bytes = content.toUtf8();
    DictRepoFactory::create_dict_repo_from_binary(bytes, "test_dict2");
    auto dict_repo = DictRepoFactory::create_dict_repo(FileSystemUtils::get_storage_dir() + "test_dict2", DictRepoEnum::Json);
    QVERIFY(dict_repo->get_byte_array().size() > 0);
}

void DictRepoFactoryTest::test_rename_dict_repo()
{
    DictRepoFactory::rename_dict_repo("test_large_dict.json", "test_large_dict_new.json");
    QVERIFY(FileSystemUtils().is_file_exist_from_standard("test_large_dict_new.json"));
}
