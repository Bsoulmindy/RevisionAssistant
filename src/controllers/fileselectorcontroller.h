#ifndef FILESELECTORCONTROLLER_H
#define FILESELECTORCONTROLLER_H

#include <QObject>
#include <QQmlEngine>

class FileSelectorController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString file_name READ file_name WRITE setFile_name NOTIFY file_nameChanged FINAL)
    QML_ELEMENT
public:
    FileSelectorController();

    Q_INVOKABLE void openFile(QString filter);
    Q_INVOKABLE QByteArray getFileBytes();

    Q_INVOKABLE void openFile(QString filter, bool useMock);

    QString file_name() const;
    void setFile_name(const QString &newFile_name);
signals:
    void file_nameChanged();
private:
    QString m_file_name;
    QByteArray m_file_bytes;
};

#endif // FILESELECTORCONTROLLER_H
