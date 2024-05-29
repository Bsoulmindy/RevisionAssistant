#ifndef FILESYSTEMUTILS_H
#define FILESYSTEMUTILS_H
#include <QString>
#include <QQmlEngine>

class FileSystemUtils : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    FileSystemUtils();
    static void prepare_storage();
    static void save_changes_to_storage();
    static QString get_storage_dir();
    static bool remove_file(const QString file_name);
    static void write_file(QString file_path, QByteArray bytes);
    static void read_file_with_function(QString file_path, QString new_file_path, std::function<void(const QString&, const QByteArray&)> callback);
    Q_INVOKABLE bool is_file_exist_from_standard(const QString file_path);
    static void remove_all_files();
};

#endif // FILESYSTEMUTILS_H
