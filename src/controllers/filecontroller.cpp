#include "filecontroller.h"
#include "dictcontroller.h"
#include <QFile>
#include <QString>
#include <QTimer>
#include "../exceptions/repo_exception.h"

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
    processFile(file_path, line_separator);
}

void FileController::constructDictFromBytes(QByteArray bytes, QString line_separator)
{
    setactualState("Preparing...");
    processBytes(bytes, line_separator);
}

void FileController::constructMToMDictFromFile(QString file_path, QString line_separator, QString entry_separator)
{
    setactualState("Preparing...");
    processFileMToM(file_path, line_separator, entry_separator);
}

void FileController::constructMToMDictFromBytes(QByteArray bytes, QString line_separator, QString entry_separator)
{
    setactualState("Preparing...");
    processBytesMToM(bytes, line_separator, entry_separator);
}

void FileController::processFile(QString file_path, QString line_separator)
{
    try {
        setactualState("Opening the file...");
        QFile file(QUrl(file_path).toLocalFile());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        processBytes(file.readAll(), line_separator);
    } catch (RepoException& e) {
        emit error(e.what());
        qCritical() << e.what();
    } catch (std::exception& e) {
        emit error("");
        qCritical() << e.what();
    }
}

void FileController::processBytes(QByteArray bytes, QString line_separator)
{
    try {
        setactualState("Fetching checked questions and responses from current database...");

        auto qst_rsp_entries_set = m_dict_controller->getCheckedQuestionsAndResponses();
        auto checked_questions = qst_rsp_entries_set.get_questions_set();
        auto checked_responses = qst_rsp_entries_set.get_responses_set();

        QTextStream file_stream(bytes);
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

            line_number++;
        }
        setactualState("Saving to the database");
        m_dict_controller->overrideDict(dict_rows_to_be_inserted);
    } catch (RepoException& e) {
        emit error(e.what());
        qCritical() << e.what();
    } catch (std::exception& e) {
        emit error("");
        qCritical() << e.what();
    }
    setactualState("Finished");
}

void FileController::processFileMToM(QString file_path, QString line_separator, QString entry_separator)
{
    try {
        setactualState("Opening the file...");
        QFile file(QUrl(file_path).toLocalFile());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        processBytesMToM(file.readAll(), line_separator, entry_separator);
    } catch (RepoException& e) {
        emit error(e.what());
        qCritical() << e.what();
    } catch (std::exception& e) {
        emit error("");
        qCritical() << e.what();
    }
}

void FileController::processBytesMToM(QByteArray bytes, QString line_separator, QString entry_separator)
{
    try {
        QTextStream file_stream(bytes);
        int line_number = 1;

        std::unordered_map<QString, std::unordered_set<QString>> question_entries;
        std::unordered_map<QString, std::unordered_set<QString>> response_entries;
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

            // Checking question and response entries
            auto question_parts = question.split(entry_separator);
            auto response_parts = response.split(entry_separator);

            // Add question entries to variables
            for(auto& question : question_parts) {
                for(auto& response : response_parts) {
                    question_entries[question].insert(response);
                    response_entries[response].insert(question);
                }
            }

            line_number++;
        }

        // Adding question lines
        std::vector<QVariantMap> dict_question_rows_to_be_inserted;
        for(auto& [question, responses] : question_entries) {
            for(auto& response : responses) {
                QVariantMap row;
                row["question"] = question;
                row["response"] = response;
                row["isCheckedQuestion"] = false;
                row["isCheckedResponse"] = false;
                dict_question_rows_to_be_inserted.push_back(row);
            }
        }

        // Adding response lines
        std::vector<QVariantMap> dict_response_rows_to_be_inserted;
        for(auto& [response, questions] : response_entries) {
            for(auto& question : questions) {
                QVariantMap row;
                row["question"] = question;
                row["response"] = response;
                row["isCheckedQuestion"] = false;
                row["isCheckedResponse"] = false;
                dict_response_rows_to_be_inserted.push_back(row);
            }
        }

        setactualState("Saving to the database");
        m_dict_controller->overrideDictMToM(dict_question_rows_to_be_inserted, dict_response_rows_to_be_inserted);
    } catch (RepoException& e) {
        emit error(e.what());
        qCritical() << e.what();
    } catch (std::exception& e) {
        emit error("");
        qCritical() << e.what();
    }
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
