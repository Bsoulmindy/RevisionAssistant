#ifndef DICTFILESCONTROLLER_H
#define DICTFILESCONTROLLER_H

#include <QObject>
#include <QQmlEngine>
#include "../models/dict_repo_enum.h"
#include "dictcontroller.h"

const char DEFAULT_DICT_FILE_NAME[] = "DefaultDictFileName";

class DictFilesController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(DictController* dict_controller READ dict_controller WRITE setDict_controller NOTIFY dict_controllerChanged)
    // The current selected dict file in the hwole application
    Q_PROPERTY(QString current_file READ current_file WRITE setcurrent_file NOTIFY current_fileChanged FINAL)
    // The temporary selected file from the list in order to execute some of the public invokable functions below
    Q_PROPERTY(QString selected_file READ selected_file WRITE setselected_file NOTIFY selected_fileChanged FINAL)
    Q_PROPERTY(QString default_file READ default_file WRITE setdefault_file NOTIFY default_fileChanged FINAL)
    Q_PROPERTY(QVariantList files READ files WRITE setfiles NOTIFY filesChanged FINAL)
    QML_ELEMENT
public:
    explicit DictFilesController(QObject *parent = nullptr);

    Q_INVOKABLE QString get_file_name_without_extension(QString full_path) const;
    Q_INVOKABLE void select_file(QString file_name);
    Q_INVOKABLE void add_new_empty_file(QString file_name, DictRepoEnum file_type);
    Q_INVOKABLE void use_database_file(QString file_name);
    Q_INVOKABLE void remove_database_file(QString file_name);
    Q_INVOKABLE void mark_default_database_file(QString file_name);
    Q_INVOKABLE void export_database_file();
    Q_INVOKABLE void export_database_file(bool useMock);
    Q_INVOKABLE void import_database_file();
    Q_INVOKABLE void import_database_file(bool useMock, QString mock_file_name);
    Q_INVOKABLE void rename_database_file(QString new_file_name);
    Q_INVOKABLE void init();

    DictController *dict_controller() const;
    void setDict_controller(DictController *newDict_controller);

    QString current_file() const;
    void setcurrent_file(const QString &newCurrent_file);

    QVariantList files() const;
    void setfiles(const QVariantList &newFiles);

    QString selected_file() const;
    void setselected_file(const QString &newSelected_file);

    QString default_file() const;
    void setdefault_file(const QString &newDefault_file);

signals:
    void dict_controllerChanged();
    void current_fileChanged();

    void filesChanged();

    void selected_fileChanged();
    void error(const QString &message);

    void default_fileChanged();

private:
    void detect_present_files();
    DictRepoEnum get_dict_type(QString full_path) const;
    QString get_file_name_wthout_path(QString file_path) const;
    QString get_extension(QString file_name) const;

    DictController *m_dict_controller = nullptr;
    QString m_current_file;
    QVariantList m_files;
    QString m_selected_file;
    QString m_default_file;
};

#endif // DICTFILESCONTROLLER_H
