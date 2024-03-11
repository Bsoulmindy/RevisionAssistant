#include "fileselectorcontroller.h"
#include <QFileDialog>

FileSelectorController::FileSelectorController() {}

void FileSelectorController::openFile(QString filter)
{
    auto fileContentReady = [&](const QString &fileName, const QByteArray &fileContent) {
        if (!fileName.isEmpty()) {
            setFile_name(fileName);
            m_file_bytes = fileContent;
        }
    };
    QFileDialog::getOpenFileContent(filter, fileContentReady);
}

QByteArray FileSelectorController::getFileBytes()
{
    return m_file_bytes;
}

QString FileSelectorController::file_name() const
{
    return m_file_name;
}

void FileSelectorController::setFile_name(const QString &newFile_name)
{
    if (m_file_name == newFile_name)
        return;
    m_file_name = newFile_name;
    emit file_nameChanged();
}
