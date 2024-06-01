#include "fileselectorcontroller.h"
#include <QFileDialog>
#include "../utils/filesystemutils.h"

FileSelectorController::FileSelectorController() {}

void FileSelectorController::openFile(QString filter)
{
    openFile(filter, false);
}

QByteArray FileSelectorController::getFileBytes()
{
    return m_file_bytes;
}

void FileSelectorController::openFile(QString filter, bool useMock)
{
    auto fileContentReady = [&](const QString &fileName, const QByteArray &fileContent) {
        if (!fileName.isEmpty()) {
            setFile_name(fileName);
            m_file_bytes = fileContent;
        }
    };
    if(useMock)
        FileSystemUtils::read_file_with_function(
            FileSystemUtils::get_storage_dir() + "test_mapper_basic.txt",
            FileSystemUtils::get_storage_dir() + "test_mapper_basic.txt",
            fileContentReady);
    else
        QFileDialog::getOpenFileContent(filter, fileContentReady);
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
