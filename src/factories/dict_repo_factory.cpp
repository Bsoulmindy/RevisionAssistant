#include "dict_repo_factory.h"
#include <QStandardPaths>
#include "../repositories/dict_json_repo.h"

std::unique_ptr<DictRepoInterface> DictRepoFactory::create_dict_repo(int id, DictRepoEnum type)
{
    QString file_name = "";
    appendTypeToFileName(file_name, type);
    appendIdToFileName(file_name, id);
    appendExtensionToFileName(file_name, type);
#if defined(Q_OS_WASM)
    QString file_dir = "/src/";
#else
    QString file_dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/";
#endif
    switch(type) {
    case DictRepoEnum::Json:
        return std::make_unique<DictJsonRepo>(file_dir + file_name);
    }
}

void DictRepoFactory::appendTypeToFileName(QString &file_name, DictRepoEnum type)
{
    switch(type) {
    case DictRepoEnum::Json:
        file_name += "json";
    }
}

void DictRepoFactory::appendIdToFileName(QString &file_name, int id)
{
    file_name += QString::number(id);
}

void DictRepoFactory::appendExtensionToFileName(QString &file_name, DictRepoEnum type)
{
    switch(type) {
    case DictRepoEnum::Json:
        file_name += ".json";
    }
}
