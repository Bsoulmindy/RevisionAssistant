#include "dict_repo_factory.h"
#include <QStandardPaths>
#include "../repositories/dict_json_repo.h"
#include "../exceptions/fileexception.h"
#include <QFile>

#if defined(Q_OS_WASM)
#include <emscripten.h>
#endif

std::unique_ptr<DictRepoInterface> DictRepoFactory::create_dict_repo(QString file_name, DictRepoEnum type)
{
    appendExtensionToFileName(file_name, type);
    QString file_dir = get_repos_dir();
    switch(type) {
    case DictRepoEnum::Json:
        return std::make_unique<DictJsonRepo>(file_dir + file_name);
    }
}

void DictRepoFactory::create_dict_repo_from_binary(const QByteArray &binary, QString file_name)
{
    QString file_path = get_repos_dir() + file_name;
    QFile file(file_path);
    if(file.open(QIODevice::ReadOnly)) {
        throw FileException("File still exists, please remove it or rename it : " + file_path);
    }
    if(!file.open(QIODevice::WriteOnly)) {
        throw FileException("Cannot create the file : " + file_path);
    }
    QTextStream file_stream(&file);
    file_stream << binary;

#if defined(Q_OS_WASM)
    EM_ASM(
        FS.syncfs(false, function (err) {
                if(err)
                    console.error(err);
            });
        );
#endif
}

void DictRepoFactory::rename_dict_repo(const QString &old_file_name, const QString &new_file_name)
{
    QString file_dir = get_repos_dir();
    if(!QFile::rename(file_dir + old_file_name, file_dir + new_file_name)) {
        throw FileException("Cannot rename to : " + new_file_name);
    }
#if defined(Q_OS_WASM)
    EM_ASM(
        FS.syncfs(false, function (err) {
                if(err)
                    console.error(err);
            });
        );
#endif
}

void DictRepoFactory::appendExtensionToFileName(QString &file_name, DictRepoEnum type)
{
    switch(type) {
    case DictRepoEnum::Json:
        file_name += ".json";
    }
}

QString DictRepoFactory::get_repos_dir() noexcept
{
#if defined(Q_OS_WASM)
    return "/src/";
#else
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/";
#endif
}
