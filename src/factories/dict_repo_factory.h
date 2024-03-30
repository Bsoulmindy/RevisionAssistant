#ifndef DICTREPOFACTORY_H
#define DICTREPOFACTORY_H

#include "../repositories/dict_repo_interface.h"
#include "../models/dict_repo_enum.h"

class DictRepoFactory
{
public:
    static std::unique_ptr<DictRepoInterface> create_dict_repo(QString file_name, DictRepoEnum type);
    static void create_dict_repo_from_binary(const QByteArray& binary, QString file_name);
private:
    static void appendExtensionToFileName(QString& file_name, DictRepoEnum type);
    static QString get_repos_dir() noexcept;
};

#endif // DICTREPOFACTORY_H
