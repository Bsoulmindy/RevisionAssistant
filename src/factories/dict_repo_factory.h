#ifndef DICTREPOFACTORY_H
#define DICTREPOFACTORY_H

#include "../repositories/dict_repo_interface.h"
#include "../models/dict_repo_enum.h"

class DictRepoFactory
{
public:
    static std::unique_ptr<DictRepoInterface> create_dict_repo(QString file_name, DictRepoEnum type);
private:
    static void appendExtensionToFileName(QString& file_name, DictRepoEnum type);
};

#endif // DICTREPOFACTORY_H
