#include "tst_dict_json_repo.h"

DictJSONRepoTest::DictJSONRepoTest()
{}

void DictJSONRepoTest::run_tests()
{
    test_select_all();
    test_update_question();
    test_update_response();
    test_mark_all_entries_unchecked();
    test_delete_all();
    test_select_questions();
    test_select_responses();
    test_insert_entry();
    test_insert_multiple_entries();
    test_insert_multiple_entries_MToM();
    test_select_by_id();
    test_get_file_name();
    test_get_byte_array();
    test_delete_by_id();
    test_edit_entry();
    test_select_all_questions();
    test_select_all_responses();
    test_is_valid_question_id();
    test_is_valid_response_id();
}

void DictJSONRepoTest::test_select_all()
{
    {
        DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_dict.json");
        auto list = repo.select_all();
        QCOMPARE(list.size(), 10);
    }
    {
        DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_MToM_dict.json");
        auto list = repo.select_all();
        QCOMPARE(list.size(), 20);
    }
}

void DictJSONRepoTest::test_update_question()
{
    {
        DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_dict.json");
        {
            repo.update_question(1, true);
            auto entry = repo.select_by_id(1);
            auto map = entry.getMap();
            QVERIFY(map["isCheckedQuestion"].toBool());
        }
        {
            repo.update_question(1, false);
            auto entry = repo.select_by_id(1);
            auto map = entry.getMap();
            QVERIFY(!map["isCheckedQuestion"].toBool());
        }
    }
    {
        DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_MToM_dict.json");
        {
            repo.update_question(1, true);
            auto entry = repo.select_by_id(1);
            auto map = entry.getMap();
            QVERIFY(map["isCheckedQuestion"].toBool());
        }
        {
            repo.update_question(1, false);
            auto entry = repo.select_by_id(1);
            auto map = entry.getMap();
            QVERIFY(!map["isCheckedQuestion"].toBool());
        }
    }
}

void DictJSONRepoTest::test_update_response()
{
    {
        DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_dict.json");
        {
            repo.update_response(1, true);
            auto entry = repo.select_by_id(1);
            auto map = entry.getMap();
            QVERIFY(map["isCheckedResponse"].toBool());
        }
        {
            repo.update_response(1, false);
            auto entry = repo.select_by_id(1);
            auto map = entry.getMap();
            QVERIFY(!map["isCheckedResponse"].toBool());
        }
    }
    {
        DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_MToM_dict.json");
        {
            repo.update_response(11, true);
            auto entry = repo.select_by_id(11);
            auto map = entry.getMap();
            QVERIFY(map["isCheckedResponse"].toBool());
        }
        {
            repo.update_response(11, false);
            auto entry = repo.select_by_id(11);
            auto map = entry.getMap();
            QVERIFY(!map["isCheckedResponse"].toBool());
        }
    }
}

void DictJSONRepoTest::test_mark_all_entries_unchecked()
{
    {
        DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_dict.json");
        for(int i = 1; i < 10; i += 2) {
            repo.update_response(i, true);
        }
        repo.mark_all_entries_unchecked();
        for(int i = 1; i < 10; i += 2) {
            auto entry = repo.select_by_id(i);
            auto map = entry.getMap();
            QVERIFY(!map["isCheckedResponse"].toBool());
        }
    }
    {
        DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_MToM_dict.json");
        for(int i = 1; i < 20; i += 2) {
            if(i < 10)
                repo.update_question(i, true);
            else
                repo.update_response(i, true);
        }
        repo.mark_all_entries_unchecked();
        for(int i = 1; i < 20; i += 2) {
            auto entry = repo.select_by_id(i);
            auto map = entry.getMap();
            if(i < 10)
                QVERIFY(!map["isCheckedQuestion"].toBool());
            else
                QVERIFY(!map["isCheckedResponse"].toBool());
        }
    }
}

void DictJSONRepoTest::test_delete_all()
{
    {
        DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_large_dict.json");
        repo.delete_all();
        auto list = repo.select_all();
        QCOMPARE(list.size(), 0);
    }
    {
        DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_large_MToM_dict.json");
        repo.delete_all();
        auto list = repo.select_all();
        QCOMPARE(list.size(), 0);
    }
}

void DictJSONRepoTest::test_select_questions()
{
    {
        DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_dict.json");
        repo.mark_all_entries_unchecked();
        repo.update_question(1, true);
        repo.update_question(2, true);
        repo.update_question(3, true);
        QCOMPARE(repo.select_questions(true).size(), 3);
        QCOMPARE(repo.select_questions(false).size(), 10 - 3);
        repo.mark_all_entries_unchecked();
    }
    {
        DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_MToM_dict.json");
        repo.mark_all_entries_unchecked();
        repo.update_question(1, true);
        repo.update_question(2, true);
        repo.update_question(3, true);
        QCOMPARE(repo.select_questions(true).size(), 3);
        QCOMPARE(repo.select_questions(false).size(), 10 - 3);
        repo.mark_all_entries_unchecked();
    }
}

void DictJSONRepoTest::test_select_responses()
{
    {
        DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_dict.json");
        repo.mark_all_entries_unchecked();
        repo.update_response(1, true);
        repo.update_response(2, true);
        repo.update_response(3, true);
        QCOMPARE(repo.select_responses(true).size(), 3);
        QCOMPARE(repo.select_responses(false).size(), 10 - 3);
        repo.mark_all_entries_unchecked();
    }
    {
        DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_MToM_dict.json");
        repo.mark_all_entries_unchecked();
        repo.update_response(11, true);
        repo.update_response(12, true);
        repo.update_response(13, true);
        QCOMPARE(repo.select_responses(true).size(), 3);
        QCOMPARE(repo.select_responses(false).size(), 10 - 3);
        repo.mark_all_entries_unchecked();
    }
}

void DictJSONRepoTest::test_insert_entry()
{
    {
        DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_dict.json");
        QuestionResponseEntry entry(0, "question 10", "response 10", true, true);
        repo.insert_entry(entry);
        auto entry_inserted = repo.select_by_id(10);
        auto map = entry_inserted.getMap();
        QCOMPARE(map["question"].toString(), "question 10");
        QCOMPARE(map["response"].toString(), "response 10");
        QCOMPARE(map["isCheckedQuestion"].toBool(), true);
        QCOMPARE(map["isCheckedResponse"].toBool(), true);
        repo.delete_by_id(10);
    }
}

void DictJSONRepoTest::test_insert_multiple_entries()
{
    DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_dict.json");
    std::list<QuestionResponseEntry> l;
    for(int i = 10; i < 20; i++) {
        l.emplace_back(0, "question " + QString::number(i), "response " + QString::number(i), true, true);
    }
    repo.insert_multiple_entries(l);
    for(int i = 10; i < 20; i++) {
        auto entry_inserted = repo.select_by_id(i);
        auto map = entry_inserted.getMap();
        QCOMPARE(map["question"].toString(), "question " + QString::number(i));
        QCOMPARE(map["response"].toString(), "response " + QString::number(i));
        QCOMPARE(map["isCheckedQuestion"].toBool(), true);
        QCOMPARE(map["isCheckedResponse"].toBool(), true);
    }
    for(int i = 19; i >= 10; i--) {
        repo.delete_by_id(i);
    }
}

void DictJSONRepoTest::test_insert_multiple_entries_MToM()
{
    DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_MToM_dict.json");
    std::list<QuestionResponseEntry> questions;
    std::list<QuestionResponseEntry> responses;
    for(int i = 10; i < 20; i++) {
        questions.emplace_back(0, "question " + QString::number(i), "response " + QString::number(i), true, true);
        responses.emplace_back(0, "response " + QString::number(i), "question " + QString::number(i), true, true);
    }
    repo.insert_multiple_entries_MToM(questions, responses);
    // Should ids be : questions = [0; 20[ and responses = [20; 40[
    for(int i = 10; i < 20; i++) {
        auto entry_inserted = repo.select_by_id(i);
        auto map = entry_inserted.getMap();
        QCOMPARE(map["question"].toString(), "question " + QString::number(i));
        QCOMPARE(map["response"].toString(), "response " + QString::number(i));
        QCOMPARE(map["isCheckedQuestion"].toBool(), true);
        QCOMPARE(map["isCheckedResponse"].toBool(), true);
    }
    for(int i = 30; i < 40; i++) {
        auto entry_inserted = repo.select_by_id(i);
        auto map = entry_inserted.getMap();
        QCOMPARE(map["question"].toString(), "response " + QString::number(i - 20));
        QCOMPARE(map["response"].toString(), "question " + QString::number(i - 20));
        QCOMPARE(map["isCheckedQuestion"].toBool(), true);
        QCOMPARE(map["isCheckedResponse"].toBool(), true);
    }
    for(int i = 39; i >= 30; i--) {
        repo.delete_by_id(i);
    }
    for(int i = 19; i >= 10; i--) {
        repo.delete_by_id(i);
    }
}

void DictJSONRepoTest::test_select_by_id()
{
    {
        DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_dict.json");
        auto entry = repo.select_by_id(5);
        auto map = entry.getMap();
        QCOMPARE(map["question"].toString(), "question #6");
        QCOMPARE(map["response"].toString(), "response #6");
        QCOMPARE(map["isCheckedQuestion"].toBool(), false);
        QCOMPARE(map["isCheckedResponse"].toBool(), false);
    }
    {
        DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_MToM_dict.json");
        auto entry = repo.select_by_id(5);
        auto map = entry.getMap();
        QCOMPARE(map["question"].toString(), "question #6");
        QCOMPARE(map["response"].toString(), "response #6");
        QCOMPARE(map["isCheckedQuestion"].toBool(), false);
        QCOMPARE(map["isCheckedResponse"].toBool(), false);
    }
}

void DictJSONRepoTest::test_get_file_name()
{
    DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_dict.json");
    QCOMPARE(repo.get_file_name(), FileSystemUtils::get_storage_dir() + "test_basic_dict.json");
}

void DictJSONRepoTest::test_get_byte_array()
{
    DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_dict.json");
    auto bytes = repo.get_byte_array();
    QVERIFY(bytes.size() > 0);
}

void DictJSONRepoTest::test_delete_by_id()
{
    {
        DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_dict.json");
        QuestionResponseEntry entry(0, "question 10", "response 10", true, true);
        repo.insert_entry(entry);
        repo.delete_by_id(10);
        QCOMPARE(repo.select_all().size(), 10);
    }
    {
        DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_MToM_dict.json");
        QuestionResponseEntry entry(0, "question 10", "response 10", true, true);
        repo.insert_question(entry);
        repo.delete_by_id(10);
        QCOMPARE(repo.select_all().size(), 20);
    }
}

void DictJSONRepoTest::test_edit_entry()
{
    {
        DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_dict.json");
        repo.edit_entry(9, "questionn 11", "responsee 11");
        auto entry = repo.select_by_id(9);
        auto map = entry.getMap();
        QCOMPARE(map["question"].toString(), "questionn 11");
        QCOMPARE(map["response"].toString(), "responsee 11");
        QCOMPARE(map["isCheckedQuestion"].toBool(), false);
        QCOMPARE(map["isCheckedResponse"].toBool(), false);
        repo.edit_entry(9, "question 10", "response 10");
    }
    {
        DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_MToM_dict.json");
        repo.edit_entry(9, "questionn 11", "responsee 11");
        auto entry = repo.select_by_id(9);
        auto map = entry.getMap();
        QCOMPARE(map["question"].toString(), "questionn 11");
        QCOMPARE(map["response"].toString(), "responsee 11");
        QCOMPARE(map["isCheckedQuestion"].toBool(), false);
        QCOMPARE(map["isCheckedResponse"].toBool(), false);
        repo.edit_entry(9, "question 10", "response 10");
    }
}

void DictJSONRepoTest::test_select_all_questions()
{
    {
        DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_dict.json");
        auto entries = repo.select_all_questions();
        QCOMPARE(entries.size(), 10);
    }
    {
        DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_MToM_dict.json");
        auto entries = repo.select_all_questions();
        QCOMPARE(entries.size(), 10);
    }
}

void DictJSONRepoTest::test_select_all_responses()
{
    {
        DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_dict.json");
        auto entries = repo.select_all_responses();
        QCOMPARE(entries.size(), 10);
    }
    {
        DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_MToM_dict.json");
        auto entries = repo.select_all_responses();
        QCOMPARE(entries.size(), 10);
    }
}

void DictJSONRepoTest::test_is_valid_question_id()
{
    {
        DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_dict.json");
        // id should be in range [0;9]
        QCOMPARE(repo.is_valid_question_id(5), true);
        QCOMPARE(repo.is_valid_question_id(0), true);
        QCOMPARE(repo.is_valid_question_id(9), true);
        QCOMPARE(repo.is_valid_question_id(-1), false);
        QCOMPARE(repo.is_valid_question_id(10), false);
        QCOMPARE(repo.is_valid_question_id(15), false);
    }
    {
        DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_MToM_dict.json");
        // id should be in range [0;9]
        QCOMPARE(repo.is_valid_question_id(5), true);
        QCOMPARE(repo.is_valid_question_id(0), true);
        QCOMPARE(repo.is_valid_question_id(9), true);
        QCOMPARE(repo.is_valid_question_id(-1), false);
        QCOMPARE(repo.is_valid_question_id(10), false);
        QCOMPARE(repo.is_valid_question_id(15), false);
    }
}

void DictJSONRepoTest::test_is_valid_response_id()
{
    {
        DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_dict.json");
        // id should be in range [0;9]
        QCOMPARE(repo.is_valid_response_id(5), true);
        QCOMPARE(repo.is_valid_response_id(0), true);
        QCOMPARE(repo.is_valid_response_id(9), true);
        QCOMPARE(repo.is_valid_response_id(-1), false);
        QCOMPARE(repo.is_valid_response_id(10), false);
        QCOMPARE(repo.is_valid_response_id(15), false);
    }
    {
        DictJsonRepo repo(FileSystemUtils::get_storage_dir() + "test_basic_MToM_dict.json");
        // id should be in range [10;19] since question ids is taking [0;9]
        QCOMPARE(repo.is_valid_response_id(15), true);
        QCOMPARE(repo.is_valid_response_id(10), true);
        QCOMPARE(repo.is_valid_response_id(19), true);
        QCOMPARE(repo.is_valid_response_id(-1), false);
        QCOMPARE(repo.is_valid_response_id(9), false);
        QCOMPARE(repo.is_valid_response_id(20), false);
        QCOMPARE(repo.is_valid_response_id(25), false);
    }
}

