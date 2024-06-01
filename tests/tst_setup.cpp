#include "tst_setup.h"
#include "../src/factories/dict_repo_factory.h"
#include "../src/models/dict_repo_enum.h"
#include "../src/utils/filesystemutils.h"

Setup::Setup() {}

void Setup::global_preparation()
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
    ///
    /// Preparing a mapper file (question <-> response)
    // "test_mapper_basic.txt" which is valid file with 10 lines
    {
        QString content;
        QTextStream stream(&content);
        for (int i = 1; i <= 10; ++i) {
            stream << "question " << i << "|response " << i << "\n";
        }
        QByteArray bytes = content.toUtf8();
        FileSystemUtils::write_file(FileSystemUtils::get_storage_dir() + "test_mapper_basic.txt", bytes);
    }

    // "test_mapper_3parts.txt" which is invalid file with 2 separators (3 parts) each line
    {
        QString content;
        QTextStream stream(&content);
        for (int i = 1; i <= 10; ++i) {
            stream << "question " << i << "|res|ponse " << i << "\n";
        }
        QByteArray bytes = content.toUtf8();
        FileSystemUtils::write_file(FileSystemUtils::get_storage_dir() + "test_mapper_3parts.txt", bytes);
    }

    // "test_mapper_1part.txt" which is invalid file with 0 separator (1 part) each line
    {
        QString content;
        QTextStream stream(&content);
        for (int i = 1; i <= 10; ++i) {
            stream << "question " << i << " response " << i << "\n";
        }
        QByteArray bytes = content.toUtf8();
        FileSystemUtils::write_file(FileSystemUtils::get_storage_dir() + "test_mapper_1part.txt", bytes);
    }

    // "test_mapper_empty.txt" which is invalid file with empty lines
    {
        QString content;
        QTextStream stream(&content);
        for (int i = 1; i <= 10; ++i) {
            stream << "\n";
        }
        QByteArray bytes = content.toUtf8();
        FileSystemUtils::write_file(FileSystemUtils::get_storage_dir() + "test_mapper_empty.txt", bytes);
    }
}

void Setup::global_cleanup()
{
    FileSystemUtils::remove_all_files();
}
