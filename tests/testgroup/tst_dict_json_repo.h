#ifndef TST_DICT_JSON_REPO_H
#define TST_DICT_JSON_REPO_H

#include "tst_group_interface.h"
#include "../../src/repositories/dict_json_repo.h"
#include "../../src/utils/filesystemutils.h"

class DictJSONRepoTest : public ITestGroup
{
public:
    DictJSONRepoTest();
    void run_tests() override;
    void test_select_all();
    void test_update_question();
    void test_update_response();
    void test_mark_all_entries_unchecked();
    void test_delete_all();
    void test_select_questions();
    void test_select_responses();
    void test_insert_entry();
    void test_insert_multiple_entries();
    void test_select_by_id();
    void test_get_file_name();
    void test_get_byte_array();
    void test_delete_by_id();
    void test_edit_entry();
};

#endif // TST_DICT_JSON_REPO_H
