#include "tst_filesystemutils.h"

FileSystemUtilsTest::FileSystemUtilsTest() {}

void FileSystemUtilsTest::run_tests()
{
    FileSystemUtils::prepare_storage();

    test_write_file();
    test_read_file();
    test_remove_file();
    test_is_file_exist_from_standard();
    test_get_dir();
    test_remove_all_files();
}

void FileSystemUtilsTest::test_write_file()
{
    QString content;
    QTextStream stream(&content);
    for (int i = 1; i <= 10; ++i) {
        stream << "question " << i << "|response " << i << "\n";
    }
    QByteArray bytes = content.toUtf8();
    FileSystemUtils::write_file("test_file1.txt", bytes);
    QVERIFY(FileSystemUtils().is_file_exist_from_standard("test_file1.txt"));
}

void FileSystemUtilsTest::test_read_file()
{
    auto callback = [&](const QString &file_path_callback, const QByteArray &bytes) {
        QString readContent = QString::fromUtf8(bytes);

        QTextStream readStream(&readContent);
        int i = 1;

        while (!readStream.atEnd()) {
            QString readLine = readStream.readLine();
            QVERIFY(readLine == "question " + QString::number(i) + "|response " + QString::number(i));
            i++;
        }
    };

    // Read from the previous test
    FileSystemUtils::read_file_with_function("test_file1.txt", "test_file1.txt", callback);
}

void FileSystemUtilsTest::test_remove_file()
{
    FileSystemUtils::remove_file("test_file1.txt");
    QVERIFY(!FileSystemUtils().is_file_exist_from_standard("test_file1.txt"));
}

void FileSystemUtilsTest::test_is_file_exist_from_standard()
{
    QString content;
    QTextStream stream(&content);
    for (int i = 1; i <= 10; ++i) {
        stream << "question " << i << "|response " << i << "\n";
    }
    QByteArray bytes = content.toUtf8();
    for(int i = 1; i < 10; i++) {
        FileSystemUtils::write_file("test_file" + QString::number(i) + ".txt", bytes);
    }

    for(int i = 1; i < 10; i++) {
        QVERIFY(FileSystemUtils().is_file_exist_from_standard("test_file" + QString::number(i) + ".txt"));
    }
}

void FileSystemUtilsTest::test_get_dir()
{
    QCOMPARE(FileSystemUtils::get_storage_dir(), FileSystemUtils().get_dir());
}

void FileSystemUtilsTest::test_remove_all_files()
{
    FileSystemUtils::remove_all_files();
    for(int i = 1; i < 10; i++) {
        QVERIFY(!FileSystemUtils().is_file_exist_from_standard("test_file" + QString::number(i) + ".txt"));
    }
}
