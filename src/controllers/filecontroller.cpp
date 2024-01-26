#include "filecontroller.h"
#include "dictcontroller.h"
#include <QThread>
#include <QFile>
#include <QString>
#include <QTimer>

FileController::FileController(QObject *parent)
    : QObject{parent}
{
    m_actualState = "Idle";
    QTimer::singleShot(0, this, &FileController::init);
}


void FileController::init()
{
    emit initialized();
}

void FileController::constructDictFromFile(QString file_path, QString line_separator)
{
    setactualState("Preparing...");
    QThread *thread = QThread::create([&](QString filePath, QString lineSeparator) {
        processFile(filePath, lineSeparator);
    }, file_path, line_separator);

    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    thread->start();
}

void FileController::processFile(QString file_path, QString line_separator)
{
    setactualState("Opening the file...");
    QFile file(QUrl(file_path).toLocalFile());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    setactualState("Fetching checked questions and responses from current database...");

    auto qst_rsp_entries_set = m_dict_controller->getCheckedQuestionsAndResponses();
    auto checked_questions = qst_rsp_entries_set.get_questions_set();
    auto checked_responses = qst_rsp_entries_set.get_responses_set();

    QTextStream file_stream(&file);
    int line_number = 1;
    std::vector<QVariantMap> dict_rows_to_be_inserted;
    setactualState("Processing the file...");
    while (!file_stream.atEnd()) {

        QString line = file_stream.readLine();
        auto line_parts = line.split(line_separator);
        if(line_parts.size() < 2) {
            emit warningOutput("Warning on line " + QString::number(line_number) + " : Couldn't find the separator '" + line_separator + "', skipping...");
            line_number++;
            continue;
        } else if(line_parts.size() > 2) {
            emit warningOutput("Warning on line " + QString::number(line_number) + " : Detected multiple separators on the same line, ignoring...");
        }
        auto& question = line_parts[0];
        auto& response = line_parts[1];
        QVariantMap row;
        row["question"] = question;
        row["response"] = response;
        row["isCheckedQuestion"] = checked_questions.contains(question);
        row["isCheckedResponse"] = checked_responses.contains(response);
        dict_rows_to_be_inserted.push_back(row);

        setactualState("Processing the file... " + QString::number(line_number) + " lines processed");
        line_number++;
    }
    setactualState("Saving to the database");
    m_dict_controller->overrideDict(dict_rows_to_be_inserted);
    setactualState("Finished");
}

QString FileController::actualState() const
{
    return m_actualState;
}

void FileController::setactualState(const QString &newActualState)
{
    if (m_actualState == newActualState)
        return;
    m_actualState = newActualState;
    emit actualStateChanged();
}

DictController *FileController::dict_controller() const
{
    return m_dict_controller;
}

void FileController::setDict_controller(DictController *newDict_controller)
{
    if (m_dict_controller == newDict_controller)
        return;
    m_dict_controller = newDict_controller;
    emit dict_controllerChanged();
}

void FileController::databaseRowSaved(int num_completed_rows, int num_total_rows)
{
    if(m_actualState != "Finished")
        setactualState("Saving to the database (" + QString::number(num_completed_rows) + "/" + QString::number(num_total_rows) + ")");
}
