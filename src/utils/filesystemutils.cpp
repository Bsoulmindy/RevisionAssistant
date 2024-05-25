#include "filesystemutils.h"
#include <QStandardPaths>
#include <QFile>

#if defined(Q_OS_WASM)
#include <emscripten.h>
#endif

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
