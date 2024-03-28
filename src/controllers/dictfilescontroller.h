#ifndef DICTFILESCONTROLLER_H
#define DICTFILESCONTROLLER_H

#include <QObject>
#include <QQmlEngine>
#include "../models/dict_repo_enum.h"
#include "dictcontroller.h"

class DictFilesController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(DictController* dict_controller READ dict_controller WRITE setDict_controller NOTIFY dict_controllerChanged)
    // The current selected dict file in the hwole application
    Q_PROPERTY(QString current_file READ current_file WRITE setcurrent_file NOTIFY current_fileChanged FINAL)
    // The temporary selected file from the list in order to execute some of the public invokable functions below
    Q_PROPERTY(QString selected_file READ selected_file WRITE setselected_file NOTIFY selected_fileChanged FINAL)
    Q_PROPERTY(QVariantList files READ files WRITE setfiles NOTIFY filesChanged FINAL)
    QML_ELEMENT
public:
    explicit DictFilesController(QObject *parent = nullptr);

    Q_INVOKABLE void select_file(QString file_name);
    Q_INVOKABLE void add_new_empty_file(QString file_name, DictRepoEnum file_type);
    Q_INVOKABLE void use_database_file(QString file_name);
    Q_INVOKABLE void remove_database_file(QString file_name);
    Q_INVOKABLE void init();

    DictController *dict_controller() const;
    void setDict_controller(DictController *newDict_controller);

    QString current_file() const;
    void setcurrent_file(const QString &newCurrent_file);

    QVariantList files() const;
    void setfiles(const QVariantList &newFiles);

    QString selected_file() const;
    void setselected_file(const QString &newSelected_file);

signals:
    void dict_controllerChanged();
    void current_fileChanged();

    void filesChanged();

    void selected_fileChanged();
    void error(const QString &message);

private:
    void detect_present_files();
    QString get_file_name_without_extension(QString full_path) const;
    DictRepoEnum get_dict_type(QString full_path) const;

    DictController *m_dict_controller = nullptr;
    QString m_current_file;
    QVariantList m_files;
    QString m_selected_file;
};

#endif // DICTFILESCONTROLLER_H
