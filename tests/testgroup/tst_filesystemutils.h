#ifndef TST_FILESYSTEMUTILS_H
#define TST_FILESYSTEMUTILS_H

#include "tst_group_interface.h"
#include "../../src/utils/filesystemutils.h"

class FileSystemUtilsTest : public ITestGroup
{
public:
    FileSystemUtilsTest();
    void run_tests() override;
    void test_write_file();
    void test_read_file();
    void test_remove_file();
    void test_is_file_exist_from_standard();
    void test_get_dir();
    void test_remove_all_files();
};

#endif // TST_FILESYSTEMUTILS_H
