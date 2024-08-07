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
    // invoke init() instead after component initialization
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
    if(hint_index == -1) {
        return;
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
    if(hint_index == -1) {
        return;
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
    QSettings settings;
    QString default_file_name = m_dict_file_name.isEmpty() ? settings.value(DEFAULT_DICT_FILE_NAME, "default.json").toString() : m_dict_file_name;
    m_dict_repo = DictRepoFactory::create_dict_repo(get_file_name_without_extension(default_file_name), DictRepoEnum::Json);
    setdict_file_name(default_file_name);
    initInternalMemory();
}

bool DictController::canQuestionBeChecked(int id)
{
    try {
        return m_dict_repo->is_valid_question_id(id);
    } catch (RepoException& e) {
        qCritical() << e.what();
        emit error(e.what());
        return false;
    } catch(std::exception& e) {
        qCritical() << e.what();
        emit error("");
        return false;
    }
}

bool DictController::canResponseBeChecked(int id)
{
    try {
        return m_dict_repo->is_valid_response_id(id);
    } catch (RepoException& e) {
        qCritical() << e.what();
        emit error(e.what());
        return false;
    } catch(std::exception& e) {
        qCritical() << e.what();
        emit error("");
        return false;
    }
}

bool DictController::editQuestionResponse(int id, const QString new_question, const QString new_response)
{
    try {
        m_dict_repo->edit_entry(id, new_question, new_response);
    } catch (RepoException& e) {
        qCritical() << e.what();
        emit error(e.what());
        return false;
    } catch(std::exception& e) {
        qCritical() << e.what();
        emit error("");
        return false;
    }
    return true;
}

bool DictController::removeEntry(int id)
{
    try {
        m_dict_repo->delete_by_id(id);
    } catch (RepoException& e) {
        qCritical() << e.what();
        emit error(e.what());
        return false;
    } catch(std::exception& e) {
        qCritical() << e.what();
        emit error("");
        return false;
    }
    return true;
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

void DictController::overrideDictMToM(const std::vector<QVariantMap> &dict_question_rows, const std::vector<QVariantMap> &dict_response_rows)
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

    // Preparing questions
    std::list<QuestionResponseEntry> question_rows;
    for(int i = 0; i < dict_question_rows.size(); i++) {
        const QVariantMap& dict_row = dict_question_rows[i];

        question_rows.emplace_back(0,
                       dict_row["question"].toString(),
                       dict_row["response"].toString(),
                       dict_row["isCheckedQuestion"].toBool(),
                       dict_row["isCheckedResponse"].toBool());
    }

    // Preparing responses
    std::list<QuestionResponseEntry> response_rows;
    for(int i = 0; i < dict_response_rows.size(); i++) {
        const QVariantMap& dict_row = dict_response_rows[i];

        response_rows.emplace_back(0,
                       dict_row["question"].toString(),
                       dict_row["response"].toString(),
                       dict_row["isCheckedQuestion"].toBool(),
                       dict_row["isCheckedResponse"].toBool());
    }

    try {
        m_dict_repo->insert_multiple_entries_MToM(question_rows, response_rows);
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
    m_num_questions = 0;
    m_num_responses = 0;
    m_not_checked_qsts.clear();
    m_not_checked_rsps.clear();

    if (m_dict_repo->get_mode() == DictModeEnum::OneToOne) {
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
            m_num_questions++;
            m_num_responses++;
        }
    } else if(m_dict_repo->get_mode() == DictModeEnum::ManyToMany) {
        std::list<QuestionResponseEntry> questions;
        std::list<QuestionResponseEntry> responses;

        try {
            questions = m_dict_repo->select_all_questions();
            responses = m_dict_repo->select_all_responses();
        } catch (RepoException& e) {
            qCritical() << e.what();
            emit error(e.what());
        } catch(std::exception& e) {
            qCritical() << e.what();
            emit error("");
        }

        for(auto& question : questions) {
            auto m = question.getMap();
            if(!m["isCheckedQuestion"].toBool()) {
                m_not_checked_qsts.push_back(question);
            }
            m_num_rows++;
            m_num_questions++;
        }

        for(auto& response : responses) {
            auto m = response.getMap();
            if(!m["isCheckedResponse"].toBool()) {
                m_not_checked_rsps.push_back(response);
            }
            m_num_rows++;
            m_num_responses++;
        }
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

bool DictController::insertNewEntry(const QString question, const QString response, bool isQuestionChecked, bool isResponseChecked)
{
    QuestionResponseEntry entry(-1, question, response, isQuestionChecked, isResponseChecked);
    try {
        m_dict_repo->insert_entry(entry);
    } catch (RepoException& e) {
        qCritical() << e.what();
        emit error(e.what());
        return false;
    } catch(std::exception& e) {
        qCritical() << e.what();
        emit error("");
        return false;
    }

    return true;
}

bool DictController::insertNewQuestion(const QString question, const QString response, bool isQuestionChecked, bool isResponseChecked)
{
    QuestionResponseEntry entry(-1, question, response, isQuestionChecked, isResponseChecked);
    try {
        m_dict_repo->insert_question(entry);
    } catch (RepoException& e) {
        qCritical() << e.what();
        emit error(e.what());
        return false;
    } catch(std::exception& e) {
        qCritical() << e.what();
        emit error("");
        return false;
    }

    return true;
}

bool DictController::insertNewResponse(const QString question, const QString response, bool isQuestionChecked, bool isResponseChecked)
{
    QuestionResponseEntry entry(-1, question, response, isQuestionChecked, isResponseChecked);
    try {
        m_dict_repo->insert_response(entry);
    } catch (RepoException& e) {
        qCritical() << e.what();
        emit error(e.what());
        return false;
    } catch(std::exception& e) {
        qCritical() << e.what();
        emit error("");
        return false;
    }

    return true;
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

QByteArray DictController::get_dict_content_binary() const
{
    return m_dict_repo->get_byte_array();
}

int DictController::num_questions() const
{
    return m_num_questions;
}

void DictController::set_num_questions(int newNum_questions)
{
    if (m_num_questions == newNum_questions)
        return;
    m_num_questions = newNum_questions;
    emit num_questionsChanged();
}

int DictController::num_responses() const
{
    return m_num_responses;
}

void DictController::set_num_responses(int newNum_responses)
{
    if (m_num_responses == newNum_responses)
        return;
    m_num_responses = newNum_responses;
    emit num_responsesChanged();
}
