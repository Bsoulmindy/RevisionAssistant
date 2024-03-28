#include "dict_repo_factory.h"
#include <QStandardPaths>
#include "../repositories/dict_json_repo.h"

std::unique_ptr<DictRepoInterface> DictRepoFactory::create_dict_repo(QString file_name, DictRepoEnum type)
{
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

void DictRepoFactory::appendExtensionToFileName(QString &file_name, DictRepoEnum type)
{
    switch(type) {
    case DictRepoEnum::Json:
        file_name += ".json";
    }
}
