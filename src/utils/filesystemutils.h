#ifndef FILESYSTEMUTILS_H
#define FILESYSTEMUTILS_H
#include <QString>

class FileSystemUtils
{
public:
    FileSystemUtils() = delete;
    static void prepare_storage();
    static void save_changes_to_storage();
    static QString get_storage_dir();
    static bool remove_file(const QString file_name);
};

#endif // FILESYSTEMUTILS_H
