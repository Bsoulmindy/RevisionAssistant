#include "filesystemutils.h"
#include <QStandardPaths>
#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include <QDir>

#if defined(Q_OS_WASM)
#include <emscripten.h>
#endif

FileSystemUtils::FileSystemUtils()
{

}

void FileSystemUtils::prepare_storage()
{
#if defined(Q_OS_WASM)
    // Prepare Persistent storage for WASM
    EM_ASM(
        if (!FS.analyzePath('/src').exists)
        FS.mkdir('/src');

        FS.mount(IDBFS, {}, '/src');
        FS.syncfs(true, function (err) {
                if(err)
                    console.error(err);
                Module._start();
            });
        );

#endif
}

void FileSystemUtils::save_changes_to_storage()
{
#if defined(Q_OS_WASM)
    EM_ASM(
        FS.syncfs(false, function (err) {
                if(err)
                    console.error(err);
            });
        );
#endif
}

QString FileSystemUtils::get_storage_dir()
{
#if defined(Q_OS_WASM)
    return "/src/";
#else
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/";
#endif
}

bool FileSystemUtils::remove_file(const QString file_name)
{
    QString file_dir = FileSystemUtils::get_storage_dir();
    QFile file(file_dir + file_name);
    bool isFileRemoved = file.remove();
    if(isFileRemoved)
        FileSystemUtils::save_changes_to_storage();

    return isFileRemoved;
}

void FileSystemUtils::write_file(QString file_path, QByteArray bytes) {
    QFile file(FileSystemUtils::get_storage_dir() + file_path);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(bytes);
        file.close();

        FileSystemUtils::save_changes_to_storage();
    } else {
        qCritical() << "Could not open file for writing:" << file_path << "-" << file.errorString();
    }
}

void FileSystemUtils::read_file_with_function(QString file_path_bytes, QString file_path_callback, std::function<void (const QString &, const QByteArray &)> callback) {
    file_path_bytes = FileSystemUtils::get_storage_dir() + file_path_bytes;
    QFile file(file_path_bytes);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray bytes = file.readAll();
        file.close();
        callback(FileSystemUtils::get_storage_dir() + file_path_callback, bytes);
    } else {
        qCritical() << "Could not open file for reading:" << file_path_bytes << "-" << file.errorString();
    }
}

bool FileSystemUtils::is_file_exist_from_standard(const QString file_path) {
    QFileInfo fileInfo(FileSystemUtils::get_storage_dir() + file_path);
    return fileInfo.exists() && fileInfo.isFile();
}

QString FileSystemUtils::get_dir()
{
    return FileSystemUtils::get_storage_dir();
}

void FileSystemUtils::remove_all_files()
{
    QString file_dir = FileSystemUtils::get_storage_dir();
    QDir directory(file_dir);

    if (!directory.exists()) {
        qDebug() << "Directory does not exist.";
        return;
    }

    QStringList files = directory.entryList(QDir::NoDotAndDotDot | QDir::Files);
    for (const QString &file : files) {
        // Remove each file
        if (!directory.remove(file)) {
            qDebug() << "Failed to remove file" << file;
        }
    }

    QStringList subdirectories = directory.entryList(QDir::NoDotAndDotDot | QDir::Dirs);
    for (const QString &subdir : subdirectories) {
        QDir subdirectory(directory.filePath(subdir));
        if (!subdirectory.removeRecursively()) {
            qDebug() << "Failed to remove directory" << subdir;
        }
    }
}
