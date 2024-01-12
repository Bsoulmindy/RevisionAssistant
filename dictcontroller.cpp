#include "dictcontroller.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QGuiApplication>
#include <QRandomGenerator>
#include <QStandardPaths>
#include <QFile>
#include <QSqlError>
#include <unordered_set>

DictController::DictController(QObject *parent)
    : QObject{parent}
{
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString databasePath = dataDir + "/dict.sqlite";
    if (!QFile::exists(databasePath))
    {
        qCritical() << "Error: " << databasePath << " doesn't exist!";
        exit(30001);
        return;
    }
    m_database = QSqlDatabase::database("revision_app");
    m_database.setDatabaseName(databasePath);
    if (!m_database.open()) {
        qCritical() << "Error: Could not open database in " << databasePath;
        exit(30002);
        return;
    }

    initInternalMemory();
}

QVariantMap DictController::selectRandomQuestion()
{
    if (m_not_checked_qsts.empty()) {
        // TODO : throw instead of returning an empty value
        return QVariantMap();
    }

    int randomIndex = QRandomGenerator::global()->bounded(static_cast<int>(m_not_checked_qsts.size()));
    QVariantMap m = m_not_checked_qsts[randomIndex];
    m["id_memory"] = randomIndex;
    return m;
}

QVariantMap DictController::selectRandomResponse()
{
    if (m_not_checked_rsps.empty()) {
        // TODO : throw instead of returning an empty value
        return QVariantMap();
    }

    int randomIndex = QRandomGenerator::global()->bounded(static_cast<int>(m_not_checked_rsps.size()));
    QVariantMap m = m_not_checked_rsps[randomIndex];
    m["id_memory"] = randomIndex;
    return m;
}

QVariantList DictController::getAllRecords()
{
    QVariantList records_list;

    QSqlQuery query("SELECT * FROM dict", m_database);
    if(query.exec()) {
        while(query.next()) {
            QSqlRecord record = query.record();
            QVariantMap resultMap;
            resultMap["question"] = record.value("question").toString();
            resultMap["response"] = record.value("response").toString();
            resultMap["isCheckedQuestion"] = record.value("isCheckedQuestion").toBool();
            resultMap["isCheckedResponse"] = record.value("isCheckedResponse").toBool();
            resultMap["id"] = record.value("id").toInt();

            records_list.append(resultMap);
        }
    } else {
        qCritical() << "Error executing SELECT query:" << query.lastError().text();
    }

    return records_list;
}

void DictController::checkQuestion(int id, int hint_index)
{
    if(hint_index == -1 || hint_index < 0 || hint_index >= m_not_checked_qsts.size()) {
        hint_index = m_not_checked_qsts.size() - 1;
    }

    // Simple Check
    {
        auto& rowAtHint = m_not_checked_qsts[hint_index];
        if(rowAtHint["id"] != id) {
            qWarning() << "The hint is wrong! The given id is " << id << " while I found at hint location : " << rowAtHint["id"];

            // Attempt to correct
            hint_index = m_not_checked_qsts.size() - 1;
            if(m_not_checked_qsts[hint_index]["id"] != id) return;
        }
    }

    std::swap(m_not_checked_qsts[hint_index], m_not_checked_qsts.back());
    m_not_checked_qsts.pop_back();

    QSqlQuery query(m_database);
    query.prepare("UPDATE dict SET isCheckedQuestion = :isChecked WHERE id = :id");
    query.bindValue(":isChecked", true);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qWarning() << "Failed to update record:" << query.lastError().text();
        return;
    }

    set_num_not_checked_questions(m_not_checked_qsts.size());
}

void DictController::checkResponse(int id, int hint_index)
{
    if(hint_index == -1 || hint_index < 0 || hint_index >= m_not_checked_rsps.size()) {
        hint_index = m_not_checked_rsps.size() - 1;
    }

    // Simple Check
    {
        auto& rowAtHint = m_not_checked_rsps[hint_index];
        if(rowAtHint["id"] != id) {
            qWarning() << "The hint is wrong! The given id is " << id << " while I found at hint location : " << rowAtHint["id"];

            // Attempt to correct
            hint_index = m_not_checked_qsts.size() - 1;
            if(m_not_checked_qsts[hint_index]["id"] != id) return;
        }
    }

    std::swap(m_not_checked_rsps[hint_index], m_not_checked_rsps.back());
    m_not_checked_rsps.pop_back();

    QSqlQuery query(m_database);
    query.prepare("UPDATE dict SET isCheckedResponse = :isChecked WHERE id = :id");
    query.bindValue(":isChecked", true);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qWarning() << "Failed to update record:" << query.lastError().text();
        return;
    }

    set_num_not_checked_responses(m_not_checked_rsps.size());
}

void DictController::uncheckQuestion(int id)
{
    QSqlQuery query(m_database);
    query.prepare("UPDATE dict SET isCheckedQuestion = :isChecked WHERE id = :id");
    query.bindValue(":isChecked", false);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qWarning() << "Failed to update record:" << query.lastError().text();
        return;
    }

    query.clear();
    query.prepare("SELECT * FROM dict WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        QSqlRecord record = query.record();
        QVariantMap result;
        result["question"] = record.value("question").toString();
        result["response"] = record.value("response").toString();
        result["isCheckedQuestion"] = record.value("isCheckedQuestion").toBool();
        result["isCheckedResponse"] = record.value("isCheckedResponse").toBool();
        result["id"] = record.value("id").toInt();

        m_not_checked_qsts.push_back(result);
    } else {
        qWarning() << "Failed to select record:" << query.lastError().text();
        return;
    }

    set_num_not_checked_questions(m_not_checked_qsts.size());
}

void DictController::uncheckResponse(int id)
{
    QSqlQuery query(m_database);
    query.prepare("UPDATE dict SET isCheckedResponse = :isChecked WHERE id = :id");
    query.bindValue(":isChecked", false);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qWarning() << "Failed to update record:" << query.lastError().text();
        return;
    }

    query.clear();
    query.prepare("SELECT * FROM dict WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        QSqlRecord record = query.record();
        QVariantMap result;
        result["question"] = record.value("question").toString();
        result["response"] = record.value("response").toString();
        result["isCheckedQuestion"] = record.value("isCheckedQuestion").toBool();
        result["isCheckedResponse"] = record.value("isCheckedResponse").toBool();
        result["id"] = record.value("id").toInt();

        m_not_checked_rsps.push_back(result);
    } else {
        qWarning() << "Failed to select record:" << query.lastError().text();
        return;
    }

    set_num_not_checked_responses(m_not_checked_rsps.size());
}

void DictController::checkQuestionInDatabase(int id)
{
    QSqlQuery query(m_database);
    query.prepare("UPDATE dict SET isCheckedQuestion = :isChecked WHERE id = :id");
    query.bindValue(":isChecked", true);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qWarning() << "Failed to update record:" << query.lastError().text();
        return;
    }
}

void DictController::checkResponseInDatabase(int id)
{
    QSqlQuery query(m_database);
    query.prepare("UPDATE dict SET isCheckedResponse = :isChecked WHERE id = :id");
    query.bindValue(":isChecked", true);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qWarning() << "Failed to update record:" << query.lastError().text();
        return;
    }
}

void DictController::uncheckQuestionInDatabase(int id)
{
    QSqlQuery query(m_database);
    query.prepare("UPDATE dict SET isCheckedQuestion = :isChecked WHERE id = :id");
    query.bindValue(":isChecked", false);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qWarning() << "Failed to update record:" << query.lastError().text();
        return;
    }
}

void DictController::uncheckResponseInDatabase(int id)
{
    QSqlQuery query(m_database);
    query.prepare("UPDATE dict SET isCheckedResponse = :isChecked WHERE id = :id");
    query.bindValue(":isChecked", false);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qWarning() << "Failed to update record:" << query.lastError().text();
        return;
    }
}

void DictController::resetDict()
{
    QSqlQuery query(m_database);
    query.prepare("UPDATE dict SET isCheckedQuestion = 0, isCheckedResponse = 0");

    if (!query.exec()) {
        qWarning() << "Failed to reset all records:" << query.lastError().text();
        return;
    }

    initInternalMemory();
}

void DictController::init()
{
    initInternalMemory();
}

void DictController::overrideDict(const std::vector<QVariantMap>& dict_rows)
{
    QSqlQuery query(m_database);
    query.prepare("DELETE FROM dict");

    if (!query.exec()) {
        qWarning() << "Failed to delete all records:" << query.lastError().text();
        return;
    }
    query.clear();

    for(int i = 0; i < dict_rows.size(); i++) {
        const QVariantMap& dict_row = dict_rows[i];
        query.prepare("INSERT INTO dict(question, response, isCheckedQuestion, isCheckedResponse) VALUES (:question, :response, :isCheckedQuestion, :isCheckedResponse)");
        query.bindValue(":question", dict_row["question"]);
        query.bindValue(":response", dict_row["response"]);
        query.bindValue(":isCheckedQuestion", dict_row["isCheckedQuestion"]);
        query.bindValue(":isCheckedResponse", dict_row["isCheckedResponse"]);

        if (!query.exec()) {
            qWarning() << "Failed to insert a row :" << query.lastError().text();
        }
        emit databaseRowInserted(i + 1, dict_rows.size());
    }

    initInternalMemory();
}

std::pair<std::unordered_set<QString>, std::unordered_set<QString> > DictController::getCheckedQuestionsAndResponses()
{
    std::unordered_set<QString> checked_questions;
    std::unordered_set<QString> checked_responses;

    QSqlQuery query(m_database);
    query.prepare("SELECT question FROM dict WHERE isCheckedQuestion = 1");

    if (query.exec()) {
        while(query.next()) {
            QSqlRecord record = query.record();
            checked_questions.insert(record.value("question").toString());
        }
    } else {
        qWarning() << "Failed to fetch checked questions : " << query.lastError().text();
    }

    query.clear();
    query.prepare("SELECT response FROM dict WHERE isCheckedResponse = 1");
    if (query.exec()) {
        while(query.next()) {
            QSqlRecord record = query.record();
            checked_responses.insert(record.value("response").toString());
        }
    } else {
        qWarning() << "Failed to fetch checked responses : " << query.lastError().text();
    }

    return {checked_questions, checked_responses};
}

void DictController::initInternalMemory()
{
    m_num_rows = 0;
    m_not_checked_qsts.clear();
    m_not_checked_rsps.clear();

    QSqlQuery query("SELECT * FROM dict", m_database);
    if(query.exec()) {
        while(query.next()) {
            QSqlRecord record = query.record();
            QVariantMap resultMap;
            resultMap["question"] = record.value("question").toString();
            resultMap["response"] = record.value("response").toString();
            resultMap["isCheckedQuestion"] = record.value("isCheckedQuestion").toBool();
            resultMap["isCheckedResponse"] = record.value("isCheckedResponse").toBool();
            resultMap["id"] = record.value("id").toInt();

            if(!resultMap["isCheckedQuestion"].toBool()) {
                m_not_checked_qsts.push_back(resultMap);
            }
            if(!resultMap["isCheckedResponse"].toBool()) {
                m_not_checked_rsps.push_back(resultMap);
            }
            m_num_rows++;
        }
    } else {
        qCritical() << "Error executing SELECT query:" << query.lastError().text();
        exit(30003);
        return;
    }

    set_num_not_checked_questions(m_not_checked_qsts.size());
    set_num_not_checked_responses(m_not_checked_rsps.size());
}

int DictController::num_rows() const
{
    return m_num_rows;
}

void DictController::set_num_rows(int newNum_rows)
{
    if (m_num_rows == newNum_rows)
        return;
    m_num_rows = newNum_rows;
    emit num_rowsChanged();
}

int DictController::num_not_checked_questions() const
{
    return m_num_not_checked_questions;
}

void DictController::set_num_not_checked_questions(int newNum_not_checked_questions)
{
    if (m_num_not_checked_questions == newNum_not_checked_questions)
        return;
    m_num_not_checked_questions = newNum_not_checked_questions;
    emit num_not_checked_questionsChanged();
}

int DictController::num_not_checked_responses() const
{
    return m_num_not_checked_responses;
}

void DictController::set_num_not_checked_responses(int newNum_not_checked_responses)
{
    if (m_num_not_checked_responses == newNum_not_checked_responses)
        return;
    m_num_not_checked_responses = newNum_not_checked_responses;
    emit num_not_checked_responsesChanged();
}
