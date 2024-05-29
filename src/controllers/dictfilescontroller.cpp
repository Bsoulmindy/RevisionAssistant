#include "dictfilescontroller.h"
#include "../factories/dict_repo_factory.h"
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QSettings>
#include <QFileDialog>
#include "../utils/filesystemutils.h"

DictFilesController::DictFilesController(QObject *parent)
    : QObject{parent}
{}

void DictFilesController::init()
{
    if(m_dict_controller == nullptr) return;

    QSettings settings;
    setdefault_file(settings.value(DEFAULT_DICT_FILE_NAME, "default.json").toString());

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
            map["isCurrent"] = false;
            m_files[i].setValue(map);
            break;
        }
    }
    setcurrent_file(file_name);
    // Use the new file
    for(int i = 0; i < m_files.size(); i++) {
        QVariantMap map = m_files[i].toMap();
        if(map["real_name"].toString() == m_current_file) {
            map["isCurrent"] = true;
            m_files[i].setValue(map);
            break;
        }
    }
    emit filesChanged();
}

void DictFilesController::remove_database_file(QString file_name)
{
    if(m_current_file == file_name) {
        emit error("Cannot remove the dictionnary that is currently being used! Please change to another dictionnary before removing.");
        return;
    }
    if(file_name == m_default_file) {
        emit error("Cannot remove the default dictionnary! Please mark another dictionnary to default before removing it.");
        return;
    }
    FileSystemUtils::remove_file(file_name);
    setselected_file(m_current_file);
    detect_present_files();
}

void DictFilesController::mark_default_database_file(QString file_name)
{
    QSettings settings;
    settings.setValue(DEFAULT_DICT_FILE_NAME, file_name);
    setdefault_file(file_name);
}

void DictFilesController::export_database_file()
{
    export_database_file(false);
}

void DictFilesController::export_database_file(bool useMock)
{
    QString file_dir = FileSystemUtils::get_storage_dir();
    QFile file(file_dir + m_selected_file);
    if(!file.open(QIODevice::ReadOnly)) {
        emit error("Cannot export file : " + m_selected_file);
        return;
    }
    if(useMock)
        FileSystemUtils::write_file(FileSystemUtils::get_storage_dir() + m_selected_file + ".export", file.readAll());
    else
        QFileDialog::saveFileContent(file.readAll(), m_selected_file);
}

void DictFilesController::import_database_file()
{
    import_database_file(false);
}

void DictFilesController::import_database_file(bool useMock)
{
    auto fileContentReady = [&](const QString &filePath, const QByteArray &fileContent) {
        if (filePath.isEmpty()) {
            return;
        }

        try {
            QString fileName = get_file_name_wthout_path(filePath);
            DictRepoFactory::create_dict_repo_from_binary(fileContent, fileName);
        } catch(std::exception& e) {
            emit error(e.what());
        }

        detect_present_files();
    };

    if(useMock)
        FileSystemUtils::read_file_with_function(
            FileSystemUtils::get_storage_dir() + m_selected_file + ".export",
            FileSystemUtils::get_storage_dir() + "mock.json",
            fileContentReady);
    else
        QFileDialog::getOpenFileContent("*",  fileContentReady);
}

void DictFilesController::rename_database_file(QString new_file_name)
{
    bool was_current = m_current_file == m_selected_file;
    bool was_default = m_default_file == m_selected_file;
    new_file_name += get_extension(m_selected_file);
    try {
        DictRepoFactory::rename_dict_repo(m_selected_file, new_file_name);
    } catch(std::exception& e) {
        emit error(e.what());
    }
    // Use the newly changed name
    if(was_current) {
        use_database_file(new_file_name);
    }
    if(was_default) {
        mark_default_database_file(new_file_name);
    }
    select_file(new_file_name);

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
    QString dir_path = FileSystemUtils::get_storage_dir();
    QStringList file_list = QDir(dir_path).entryList();
    for(const QString& file : file_list) {
        if(file == "." || file == "..") continue;

        QVariantMap map;
        map["real_name"] = file;
        map["name"] = get_file_name_without_extension(file);
        map["isSelected"] = m_selected_file == file;
        map["isCurrent"] = m_current_file == file;
        m_files.push_back(map);
    }
    emit filesChanged();
}

QString DictFilesController::get_file_name_without_extension(QString full_path) const
{
    // Filtering folders
    QStringList folders = full_path.split(QDir::separator());
    // Filtering extension "."
    QStringList list = folders.back().split(".");
    int s = list.size();
    if(s == 1) return list.back();
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

QString DictFilesController::get_file_name_wthout_path(QString file_path) const
{
    auto list = file_path.split("/");
    int s = list.size();
    if(s == 1) return file_path;

    return list[s-1];
}

QString DictFilesController::get_extension(QString file_name) const
{
    auto list = file_name.split(".");
    int s = list.size();
    return "." + list[s-1];
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

QString DictFilesController::default_file() const
{
    return m_default_file;
}

void DictFilesController::setdefault_file(const QString &newDefault_file)
{
    if (m_default_file == newDefault_file)
        return;
    m_default_file = newDefault_file;
    emit default_fileChanged();
}
