#include "dictcontroller.h"
#include "../models/question_response_entry.h"
#include <QGuiApplication>
#include <QRandomGenerator>
#include <QStandardPaths>
#include <QFile>
#include <unordered_set>
#include "../factories/dict_repo_factory.h"
#include <memory>

DictController::DictController(QObject *parent)
    : QObject{parent}
{
    m_dict_repo = DictRepoFactory.create_dict_repo(1, DictRepoEnum::Json);

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
    try {
        std::list<QuestionResponseEntry> entries = m_dict_repo->select_all();
        for(auto it = entries.begin(); it != entries.end(); it++) {
            record_list.append(it->getMap());
        }
    } catch (std::exception& e) {
        qCritical() << e.what();
    }
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

    // Update in memory
    std::swap(m_not_checked_qsts[hint_index], m_not_checked_qsts.back());
    m_not_checked_qsts.pop_back();

    // Save in repository
    try {
        m_dict_repo->update_question(id, true);
    } catch(std::exception e) {
        qCritical() << e.what();
    }

    // Emit signal of the change
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

    // Update in memory
    std::swap(m_not_checked_rsps[hint_index], m_not_checked_rsps.back());
    m_not_checked_rsps.pop_back();

    // Save in repository
    try {
        m_dict_repo->update_response(id, true);
    } catch(std::exception e) {
        qCritical() << e.what();
    }

    // Emit signal of the change
    set_num_not_checked_responses(m_not_checked_rsps.size());
}

void DictController::uncheckQuestion(int id)
{    
    try {
        m_dict_repo->update_question(id, false);
    } catch(std::exception e) {
        qCritical() << e.what();
        return;
    }

    try {
        QuestionResponseEntry entry = m_dict_repo->select_by_id(id);
        m_not_checked_qsts.push_back(entry.getMap());
    } catch(std::exception e) {
        qCritical() << e.what();
        return;
    }

    set_num_not_checked_questions(m_not_checked_qsts.size());
}

void DictController::uncheckResponse(int id)
{
    try {
        m_dict_repo->update_response(id, false);
    } catch(std::exception e) {
        qCritical() << e.what();
        return;
    }

    try {
        QuestionResponseEntry entry = m_dict_repo->select_by_id(id);
        m_not_checked_rsps.push_back(entry.getMap());
    } catch(std::exception e) {
        qCritical() << e.what();
        return;
    }

    set_num_not_checked_responses(m_not_checked_rsps.size());
}

void DictController::checkQuestionInDatabase(int id)
{
    try {
        m_dict_repo->update_question(id, true);
    } catch(std::exception e) {
        qCritical() << e.what();
        return;
    }
}

void DictController::checkResponseInDatabase(int id)
{
    try {
        m_dict_repo->update_response(id, true);
    } catch(std::exception e) {
        qCritical() << e.what();
        return;
    }
}

void DictController::uncheckQuestionInDatabase(int id)
{
    try {
        m_dict_repo->update_question(id, false);
    } catch(std::exception e) {
        qCritical() << e.what();
        return;
    }
}

void DictController::uncheckResponseInDatabase(int id)
{
    try {
        m_dict_repo->update_response(id, false);
    } catch(std::exception e) {
        qCritical() << e.what();
        return;
    }
}

void DictController::resetDict()
{
    try {
        m_dict_repo->mark_all_entries_unchecked();
    } catch(std::exception e) {
        qCritical() << e.what();
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
    try {
        m_dict_repo->delete_all();
    } catch(std::exception e) {
        qCritical() << e.what();
        return;
    }
    std::list<QuestionResponseEntry> l;
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

        l.emplace_back(0,
                       dict_row["question"],
                       dict_row["response"],
                       dict_row["isCheckedQuestion"],
                       dict_row["isCheckedResponse"]);

    }

    try {
        m_dict_repo->insert_multiple_entries(l);
    } catch(std::exception e) {
        qCritical() << e.what();
    }

    initInternalMemory();
}

std::pair<std::unordered_set<QString>, std::unordered_set<QString> > DictController::getCheckedQuestionsAndResponses()
{
    std::unordered_set<QString> checked_questions;
    std::unordered_set<QString> checked_responses;

    try {
        std::list<QuestionResponseEntry> entries = m_dict_repo->select_questions(true);
        for(auto& entry : entries) {
            checked_questions.insert(entry.getMap()["question"].toString());
        }
    } catch(std::exception e) {
        qCritical() << e.what();
    }

    try {
        std::list<QuestionResponseEntry> entries = m_dict_repo->select_responses(true);
        for(auto& entry : entries) {
            checked_responses.insert(entry.getMap()["response"].toString());
        }
    } catch(std::exception e) {
        qCritical() << e.what();
    }

    return {checked_questions, checked_responses};
}

void DictController::initInternalMemory()
{
    m_num_rows = 0;
    m_not_checked_qsts.clear();
    m_not_checked_rsps.clear();

    std::list<QuestionResponseEntry> entries;

    try {
        entries = m_dict_repo->select_all();
    } catch(std::exception e) {
        qCritical() << e.what();
    }

    for(auto& entry : entries) {
        if(!entry.getMap()["isCheckedQuestion"].toBool()) {
            m_not_checked_qsts.push_back(entry);
        }
        if(!entry.getMap()["isCheckedResponse"].toBool()) {
            m_not_checked_rsps.push_back(entry);
        }
        m_num_rows++;
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
