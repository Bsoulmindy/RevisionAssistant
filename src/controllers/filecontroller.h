#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#include <QObject>
#include <QQmlEngine>
#include "dictcontroller.h"

/**
 * @brief Reads the mapping between questions <-> responses
 */
class FileController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString actualState READ actualState WRITE setactualState NOTIFY actualStateChanged FINAL)
    Q_PROPERTY(DictController* dict_controller READ dict_controller WRITE setDict_controller NOTIFY dict_controllerChanged)
    QML_ELEMENT
public:
    explicit FileController(QObject *parent = nullptr);

    Q_INVOKABLE void constructDictFromFile(QString file_path, QString line_separator);
    Q_INVOKABLE void constructDictFromBytes(QByteArray bytes, QString line_separator);

    QString actualState() const;
    void setactualState(const QString &newActualState);

    DictController *dict_controller() const;
    void setDict_controller(DictController *newDict_controller);

signals:
    void warningOutput(const QString &output);
    void actualStateChanged();
    void initialized();
    void dict_controllerChanged();
    void error(const QString &message);

private:
    void init();
    void processFile(QString file_path, QString line_separator);
    void processBytes(QByteArray bytes, QString line_separator);
    QString m_actualState;
    DictController *m_dict_controller = nullptr;
};

#endif // FILECONTROLLER_H
