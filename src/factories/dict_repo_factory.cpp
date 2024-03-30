#include "dict_repo_factory.h"
#include <QStandardPaths>
#include "../repositories/dict_json_repo.h"
#include "../exceptions/fileexception.h"
#include <QFile>

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
