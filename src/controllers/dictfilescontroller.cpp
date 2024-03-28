#include "dictfilescontroller.h"
#include "../factories/dict_repo_factory.h"
#include <QFile>
#include <QDir>
#include <QStandardPaths>

QString get_directory_path() noexcept {
#if defined(Q_OS_WASM)
    return "/src/";
#else
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/";
#endif
}

const char CURRENT_SUFFIX[] = " (Current)";

DictFilesController::DictFilesController(QObject *parent)
    : QObject{parent}
{}

void DictFilesController::init()
{
    if(m_dict_controller == nullptr) return;

    setcurrent_file(m_dict_controller->get_file_name());
    detect_present_files();
    select_file(m_current_file);
}

void DictFilesController::select_file(QString file_name)
{
    // Unselect the old file
    for(int i = 0; i < m_files.size(); i++) {
        QVariantMap map = m_files[i].toMap();
        if(map["real_name"].toString() == m_selected_file) {
            map["isSelected"] = false;
            m_files[i].setValue(map);
            break;
        }
    }
    setselected_file(file_name);
    // Select the new file
    for(int i = 0; i < m_files.size(); i++) {
        QVariantMap map = m_files[i].toMap();
        if(map["real_name"].toString() == m_selected_file) {
            map["isSelected"] = true;
            m_files[i].setValue(map);
            break;
        }
    }
    emit filesChanged();
}

void DictFilesController::add_new_empty_file(QString file_name, DictRepoEnum file_type)
{
    DictRepoFactory::create_dict_repo(file_name, file_type);
    detect_present_files();
}

void DictFilesController::use_database_file(QString file_name)
{
    QString dict_name = get_file_name_without_extension(file_name);
    DictRepoEnum dict_type = get_dict_type(file_name);
    m_dict_controller->change_dict(dict_name, dict_type);

    // Unuse the old file
    for(int i = 0; i < m_files.size(); i++) {
        QVariantMap map = m_files[i].toMap();
        if(map["real_name"].toString() == m_current_file) {
            map["name"] = get_file_name_without_extension(map["real_name"].toString());
            m_files[i].setValue(map);
            break;
        }
    }
    setcurrent_file(file_name);
    // Use the new file
    for(int i = 0; i < m_files.size(); i++) {
        QVariantMap map = m_files[i].toMap();
        if(map["real_name"].toString() == m_current_file) {
            map["name"] = get_file_name_without_extension(map["real_name"].toString()) + QString(CURRENT_SUFFIX);
            m_files[i].setValue(map);
            break;
        }
    }
    emit filesChanged();
}

void DictFilesController::remove_database_file(QString file_name)
{
    if(m_current_file == file_name) {
        emit error("Cannot remove the dictionnary that is currently beign used! Please change to another dictionnary before removing.");
        return;
    }
    QString file_dir = get_directory_path();
    QFile json_file(file_dir + file_name);
    json_file.remove();

    detect_present_files();
}

DictController *DictFilesController::dict_controller() const
{
    return m_dict_controller;
}

void DictFilesController::setDict_controller(DictController *newDict_controller)
{
    if (m_dict_controller == newDict_controller)
        return;
    m_dict_controller = newDict_controller;
    emit dict_controllerChanged();
}

QString DictFilesController::current_file() const
{
    return m_current_file;
}

void DictFilesController::setcurrent_file(const QString &newCurrent_file)
{
    if (m_current_file == newCurrent_file)
        return;
    m_current_file = newCurrent_file;
    emit current_fileChanged();
}

void DictFilesController::detect_present_files()
{
    m_files.clear();
    QString dir_path = get_directory_path();
    QStringList file_list = QDir(dir_path).entryList();
    for(const QString& file : file_list) {
        if(file == "." || file == "..") continue;

        QVariantMap map;
        map["real_name"] = file;
        map["name"] = get_file_name_without_extension(file) + (m_current_file == file ? QString(CURRENT_SUFFIX) : "");
        map["isSelected"] = m_selected_file == file;
        m_files.push_back(map);
    }
    emit filesChanged();
}

QString DictFilesController::get_file_name_without_extension(QString full_path) const
{
    auto list = full_path.split(".");
    int s = list.size();
    if(s == 1) return full_path;
    QString name;
    for(int i = 0; i < s - 1; i++) {
        name += list[i];
        if(i < s - 2) name += ".";
    }
    return name;
}

DictRepoEnum DictFilesController::get_dict_type(QString full_path) const
{
    auto list = full_path.split(".");
    int s = list.size();
    if(s == 1) return DictRepoEnum::Json;

    QString extension = list[s-1];
    if(extension == "json") return DictRepoEnum::Json;
    return DictRepoEnum::Json;
}

QVariantList DictFilesController::files() const
{
    return m_files;
}

void DictFilesController::setfiles(const QVariantList &newFiles)
{
    if (m_files == newFiles)
        return;
    m_files = newFiles;
    emit filesChanged();
}

QString DictFilesController::selected_file() const
{
    return m_selected_file;
}

void DictFilesController::setselected_file(const QString &newSelected_file)
{
    if (m_selected_file == newSelected_file)
        return;
    m_selected_file = newSelected_file;
    emit selected_fileChanged();
}
