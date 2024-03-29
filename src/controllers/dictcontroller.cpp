#include "dictcontroller.h"
#include "../models/question_response_entry.h"
#include <QGuiApplication>
#include <QRandomGenerator>
#include <QStandardPaths>
#include <QFile>
#include <unordered_set>
#include "../factories/dict_repo_factory.h"
#include <memory>
#include "../exceptions/repo_exception.h"
#include "../exceptions/run_out_of_entries.h"
#include <QSettings>
#include "dictfilescontroller.h"

DictController::DictController(QObject *parent)
    : QObject{parent}
{
    QSettings settings;
    QString default_file_name = settings.value(DEFAULT_DICT_FILE_NAME, "default.json").toString();
    m_dict_repo = DictRepoFactory::create_dict_repo(get_file_name_without_extension(default_file_name), DictRepoEnum::Json);
    m_dict_file_name = default_file_name;
    initInternalMemory();
}

QString get_dict_type_string(DictRepoEnum dict_type) {
    switch(dict_type) {
    case DictRepoEnum::Json:
        return "json";
    }
}

QVariantMap DictController::selectRandomQuestion()
{
    if (m_not_checked_qsts.empty()) {
        throw RunOutOfEntries("All questions are finished!");
    }

    int randomIndex = QRandomGenerator::global()->bounded(static_cast<int>(m_not_checked_qsts.size()));
    QVariantMap m = m_not_checked_qsts[randomIndex].getMap();
    m["id_memory"] = randomIndex;
    return m;
}

QVariantMap DictController::selectRandomResponse()
{
    if (m_not_checked_rsps.empty()) {
        throw RunOutOfEntries("All responses are finished!");
    }

    int randomIndex = QRandomGenerator::global()->bounded(static_cast<int>(m_not_checked_rsps.size()));
    QVariantMap m = m_not_checked_rsps[randomIndex].getMap();
    m["id_memory"] = randomIndex;
    return m;
}

QVariantList DictController::getAllRecords(QString search_term)
{
    QVariantList records_list;
    try {
        std::list<QuestionResponseEntry> entries = m_dict_repo->select_all();
        for(auto it = entries.begin(); it != entries.end(); it++) {
            if(search_term == "" || it->getQuestion().contains(search_term) || it->getResponse().contains(search_term))
                records_list.append(it->getMap());
        }
    } catch (std::exception& e) {
        qCritical() << e.what();
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
        auto rowAtHint = m_not_checked_qsts[hint_index].getMap();
        if(rowAtHint["id"] != id) {
            qWarning() << "The hint is wrong! The given id is " << id << " while I found at hint location : " << rowAtHint["id"];

            // Attempt to correct
            hint_index = m_not_checked_qsts.size() - 1;
            if(m_not_checked_qsts[hint_index].getMap()["id"] != id) {
                emit error("");
                return;
            }
        }
    }

    // Update in memory
    std::swap(m_not_checked_qsts[hint_index], m_not_checked_qsts.back());
    m_not_checked_qsts.pop_back();

    // Save in repository
    try {
        m_dict_repo->update_question(id, true);
    } catch (RepoException& e) {
        qCritical() << e.what();
        emit error(e.what());
    } catch(std::exception& e) {
        qCritical() << e.what();
        emit error("");
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
        auto rowAtHint = m_not_checked_rsps[hint_index].getMap();
        if(rowAtHint["id"] != id) {
            qWarning() << "The hint is wrong! The given id is " << id << " while I found at hint location : " << rowAtHint["id"];

            // Attempt to correct
            hint_index = m_not_checked_qsts.size() - 1;
            if(m_not_checked_qsts[hint_index].getMap()["id"] != id) {
                emit error("");
                return;
            }
        }
    }

    // Update in memory
    std::swap(m_not_checked_rsps[hint_index], m_not_checked_rsps.back());
    m_not_checked_rsps.pop_back();

    // Save in repository
    try {
        m_dict_repo->update_response(id, true);
    } catch (RepoException& e) {
        qCritical() << e.what();
        emit error(e.what());
    } catch(std::exception& e) {
        qCritical() << e.what();
        emit error("");
    }

    // Emit signal of the change
    set_num_not_checked_responses(m_not_checked_rsps.size());
}

void DictController::uncheckQuestion(int id)
{    
    try {
        m_dict_repo->update_question(id, false);
    } catch (RepoException& e) {
        qCritical() << e.what();
        emit error(e.what());
        return;
    } catch(std::exception& e) {
        qCritical() << e.what();
        emit error("");
        return;
    }

    try {
        QuestionResponseEntry entry = m_dict_repo->select_by_id(id);
        m_not_checked_qsts.push_back(entry);
    } catch (RepoException& e) {
        qCritical() << e.what();
        emit error(e.what());
        return;
    } catch(std::exception& e) {
        qCritical() << e.what();
        emit error("");
        return;
    }

    set_num_not_checked_questions(m_not_checked_qsts.size());
}

void DictController::uncheckResponse(int id)
{
    try {
        m_dict_repo->update_response(id, false);
    } catch (RepoException& e) {
        qCritical() << e.what();
        emit error(e.what());
        return;
    } catch(std::exception& e) {
        qCritical() << e.what();
        emit error("");
        return;
    }

    try {
        QuestionResponseEntry entry = m_dict_repo->select_by_id(id);
        m_not_checked_rsps.push_back(entry);
    } catch (RepoException& e) {
        qCritical() << e.what();
        emit error(e.what());
        return;
    } catch(std::exception& e) {
        qCritical() << e.what();
        emit error("");
        return;
    }

    set_num_not_checked_responses(m_not_checked_rsps.size());
}

void DictController::checkQuestionInDatabase(int id)
{
    try {
        m_dict_repo->update_question(id, true);
    } catch (RepoException& e) {
        qCritical() << e.what();
        emit error(e.what());
        return;
    } catch(std::exception& e) {
        qCritical() << e.what();
        emit error("");
        return;
    }
}

void DictController::checkResponseInDatabase(int id)
{
    try {
        m_dict_repo->update_response(id, true);
    } catch (RepoException& e) {
        qCritical() << e.what();
        emit error(e.what());
        return;
    } catch(std::exception& e) {
        qCritical() << e.what();
        emit error("");
        return;
    }
}

void DictController::uncheckQuestionInDatabase(int id)
{
    try {
        m_dict_repo->update_question(id, false);
    } catch (RepoException& e) {
        qCritical() << e.what();
        emit error(e.what());
        return;
    } catch(std::exception& e) {
        qCritical() << e.what();
        emit error("");
        return;
    }
}

void DictController::uncheckResponseInDatabase(int id)
{
    try {
        m_dict_repo->update_response(id, false);
    } catch (RepoException& e) {
        qCritical() << e.what();
        emit error(e.what());
        return;
    } catch(std::exception& e) {
        qCritical() << e.what();
        emit error("");
        return;
    }
}

void DictController::resetDict()
{
    try {
        m_dict_repo->mark_all_entries_unchecked();
    } catch (RepoException& e) {
        qCritical() << e.what();
        emit error(e.what());
        return;
    } catch(std::exception& e) {
        qCritical() << e.what();
        emit error("");
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
    } catch (RepoException& e) {
        qCritical() << e.what();
        emit error(e.what());
        return;
    } catch(std::exception& e) {
        qCritical() << e.what();
        emit error("");
        return;
    }
    std::list<QuestionResponseEntry> l;
    for(int i = 0; i < dict_rows.size(); i++) {
        const QVariantMap& dict_row = dict_rows[i];

        l.emplace_back(0,
                       dict_row["question"].toString(),
                       dict_row["response"].toString(),
                       dict_row["isCheckedQuestion"].toBool(),
                       dict_row["isCheckedResponse"].toBool());
    }

    try {
        m_dict_repo->insert_multiple_entries(l);
    } catch (RepoException& e) {
        qCritical() << e.what();
        emit error(e.what());
    } catch(std::exception& e) {
        qCritical() << e.what();
        emit error("");
    }

    initInternalMemory();
}

QuestionResponseEntriesSet DictController::getCheckedQuestionsAndResponses()
{
    std::unordered_set<QString> checked_questions;
    std::unordered_set<QString> checked_responses;

    try {
        std::list<QuestionResponseEntry> entries = m_dict_repo->select_questions(true);
        for(auto& entry : entries) {
            auto m = entry.getMap();
            checked_questions.insert(m["question"].toString());
        }
    } catch (RepoException& e) {
        qCritical() << e.what();
        emit error(e.what());
    } catch(std::exception& e) {
        qCritical() << e.what();
        emit error("");
    }

    try {
        std::list<QuestionResponseEntry> entries = m_dict_repo->select_responses(true);
        for(auto& entry : entries) {
            auto m = entry.getMap();
            checked_responses.insert(m["response"].toString());
        }
    } catch (RepoException& e) {
        qCritical() << e.what();
        emit error(e.what());
    } catch(std::exception& e) {
        qCritical() << e.what();
        emit error("");
    }

    return QuestionResponseEntriesSet(std::move(checked_questions), std::move(checked_responses));
}

void DictController::change_dict(QString dict_name, DictRepoEnum dict_type)
{
    m_dict_repo = DictRepoFactory::create_dict_repo(dict_name, dict_type);
    setdict_file_name(dict_name + "." + get_dict_type_string(dict_type));
    initInternalMemory();
}

void DictController::initInternalMemory()
{
    m_num_rows = 0;
    m_not_checked_qsts.clear();
    m_not_checked_rsps.clear();

    std::list<QuestionResponseEntry> entries;

    try {
        entries = m_dict_repo->select_all();
    } catch (RepoException& e) {
        qCritical() << e.what();
        emit error(e.what());
    } catch(std::exception& e) {
        qCritical() << e.what();
        emit error("");
    }

    for(auto& entry : entries) {
        auto m = entry.getMap();
        if(!m["isCheckedQuestion"].toBool()) {
            m_not_checked_qsts.push_back(entry);
        }
        if(!m["isCheckedResponse"].toBool()) {
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

QString DictController::get_file_name_without_extension(QString file_name) const
{
    auto list = file_name.split(".");
    int s = list.size();
    if(s == 1) return file_name;
    QString name;
    for(int i = 0; i < s - 1; i++) {
        name += list[i];
        if(i < s - 2) name += ".";
    }
    return name;
}

QString DictController::get_file_name() const
{
    return m_dict_file_name;
}

void DictController::setdict_file_name(const QString &newDict_file_name)
{
    if (m_dict_file_name == newDict_file_name)
        return;
    m_dict_file_name = newDict_file_name;
    emit dict_file_nameChanged();
}
