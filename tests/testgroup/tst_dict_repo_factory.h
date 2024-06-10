#ifndef TST_DICT_REPO_FACTORY_H
#define TST_DICT_REPO_FACTORY_H

#include "tst_group_interface.h"
#include "../../src/factories/dict_repo_factory.h"
#include "../../src/utils/filesystemutils.h"

class DictRepoFactoryTest : public ITestGroup {
public:
    DictRepoFactoryTest();
    void run_tests() override;
    void test_create_dict_repo_on_new();
    void test_create_dict_repo_on_exist();
    void test_create_dict_repo_from_binary();
    void test_rename_dict_repo();
};

#endif // TST_DICT_REPO_FACTORY_H
