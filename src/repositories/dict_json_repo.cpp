#include "dict_json_repo.h"
#include <QFile>
#include "../exceptions/json_exception.h"
#include "../exceptions/file_invalid_json_exception.h"
#include "../models/question_response_entry.h"
#include "../exceptions/object_invalid_json_exception.h"

DictJsonRepo::DictJsonRepo(QString json_path)
{
    m_json_path = json_path;
    QFile json_file(json_path);
    if(!json_file.open(QIODevice::ReadOnly)) {
        throw FileInvalidJsonException("Cannot open the file " + json_path);
    }

    auto bytes = json_file.readAll();
    QJsonParseError json_error;
    m_json_document = QJsonDocument::fromJson(bytes, &json_error);
    if(json_error != QJsonParseError::NoError) {
        throw FileInvalidJsonException("Error occured when reading the file " + json_path + " as JSON : " + json_error.errorString());
    }
}

std::list<QuestionResponseEntry> DictJsonRepo::select_all()
{
    if(m_json_document.isNull()) {
        throw FileInvalidJsonException("The JSON Document is invalid!");
    }
    if(!m_json_document.isArray()) {
        throw FileInvalidJsonException("Invalid syntax of JSON!");
    }
    std::list<QuestionResponseEntry> all_entries();
    std::list<int> invalid_objects_ids();
    auto entries_array = m_json_document.array();
    for(int i = 0; i < entries_array.size(); i++) {
        QJsonValueRef entry_object = entries_array[i];
        if(!entry_object.isObject()) {
            invalid_objects_ids.push_back(i);
            continue;
        }
        auto question_value = entry_object["question"];
        auto response_value = entry_object["response"];
        auto is_checked_question_value = entry_object["isCheckedQuestion"];
        auto is_checked_response_value = entry_object["isCheckedResponse"];

        if(!question_value.isString() || !response_value.isString() || !is_checked_question_value.isBool() || !is_checked_response_value.isBool()) {
            invalid_objects_ids.push_back(i);
            continue;
        }
        auto question = question_value.toString();
        auto response = response_value.toString();
        auto is_checked_question = is_checked_question_value.toBool();
        auto is_checked_response = is_checked_response_value.toBool();

        all_entries.emplace_back(id, question, response, is_checked_question, is_checked_response);
    }
    // Checking if any problem occured
    if(!invalid_objects_ids.empty()) {
        throw ObjectInvalidJsonException("Some JSON objects are not valid!", std::move(invalid_objects_ids));
    }
    return all_entries;
}

void DictJsonRepo::update_question(int id, bool is_checked)
{
    if(m_json_document.isNull()) {
        throw FileInvalidJsonException("The JSON Document is invalid!");
    }
    if(!m_json_document.isArray()) {
        throw FileInvalidJsonException("Invalid syntax of JSON!");
    }
    auto entries_array = m_json_document.array();
    if(id >= entries_array.size()) {
        throw JsonException("ID " + id + " doesn't exist in JSON");
    }
    QJsonValueRef entry_object = entries_array[id];
    if(!entry_object["isCheckedQuestion"].isBool()) {
        throw ObjectInvalidJsonException("Invalid JSON Object ID " + id, {id});
    }
    entry_object["isCheckedQuestion"] = is_checked;

    save();
}

void DictJsonRepo::update_response(int id, bool is_checked)
{
    if(m_json_document.isNull()) {
        throw FileInvalidJsonException("The JSON Document is invalid!");
    }
    if(!m_json_document.isArray()) {
        throw FileInvalidJsonException("Invalid syntax of JSON!");
    }
    auto entries_array = m_json_document.array();
    if(id >= entries_array.size()) {
        throw JsonException("ID " + id + " doesn't exist in JSON");
    }
    QJsonValueRef entry_object = entries_array[id];
    if(!entry_object["isCheckedResponse"].isBool()) {
        throw ObjectInvalidJsonException("Invalid JSON Object ID " + id, {id});
    }
    entry_object["isCheckedResponse"] = is_checked;

    save();
}

void DictJsonRepo::mark_all_entries_unchecked()
{
    if(m_json_document.isNull()) {
        throw FileInvalidJsonException("The JSON Document is invalid!");
    }
    if(!m_json_document.isArray()) {
        throw FileInvalidJsonException("Invalid syntax of JSON!");
    }
    auto entries_array = m_json_document.array();
    for(int i = 0; i < entries_array.size(); i++) {
        QJsonValueRef entry_object = entries_array[i];

        if(!entry_object["isCheckedResponse"].isBool() || !entry_object["isCheckedQuestion"].isBool()) {
            throw ObjectInvalidJsonException("Invalid JSON Object ID " + id, {id});
        }

        entry_object["isCheckedResponse"] = false;
        entry_object["isCheckedQuestion"] = false;
    }

    save();
}

void DictJsonRepo::delete_all()
{
    if(m_json_document.isNull()) {
        throw FileInvalidJsonException("The JSON Document is invalid!");
    }
    if(!m_json_document.isArray()) {
        throw FileInvalidJsonException("Invalid syntax of JSON!");
    }
    auto entries_array = m_json_document.array();
    while(!entries_array.isEmpty()) {
        entries_array.pop_back();
    }

    save();
}

std::list<QuestionResponseEntry> DictJsonRepo::select_questions(bool is_checked)
{
    if(m_json_document.isNull()) {
        throw FileInvalidJsonException("The JSON Document is invalid!");
    }
    if(!m_json_document.isArray()) {
        throw FileInvalidJsonException("Invalid syntax of JSON!");
    }
    auto entries_array = m_json_document.array();
    std::list<int> invalid_objects_ids();

    for(int i = 0; i < entries_array.size(); i++) {
        QJsonValueRef entry_object = entries_array[i];
        if(!entry_object.isObject()) {
            invalid_objects_ids.push_back(i);
            continue;
        }
        auto question_value = entry_object["question"];
        auto response_value = entry_object["response"];
        auto is_checked_question_value = entry_object["isCheckedQuestion"];
        auto is_checked_response_value = entry_object["isCheckedResponse"];

        if(!question_value.isString() || !response_value.isString() || !is_checked_question_value.isBool() || !is_checked_response_value.isBool()) {
            invalid_objects_ids.push_back(i);
            continue;
        }
        // Here's the filter
        if(is_checked_question_value != is_checked) continue;

        auto question = question_value.toString();
        auto response = response_value.toString();
        auto is_checked_question = is_checked_question_value.toBool();
        auto is_checked_response = is_checked_response_value.toBool();

        all_entries.emplace_back(id, question, response, is_checked_question, is_checked_response);
    }
    // Checking if any problem occured
    if(!invalid_objects_ids.empty()) {
        throw ObjectInvalidJsonException("Some JSON objects are not valid!", std::move(invalid_objects_ids));
    }
    return all_entries;
}

std::list<QuestionResponseEntry> DictJsonRepo::select_responses(bool is_checked)
{
    if(m_json_document.isNull()) {
        throw FileInvalidJsonException("The JSON Document is invalid!");
    }
    if(!m_json_document.isArray()) {
        throw FileInvalidJsonException("Invalid syntax of JSON!");
    }
    auto entries_array = m_json_document.array();
    std::list<int> invalid_objects_ids();

    for(int i = 0; i < entries_array.size(); i++) {
        QJsonValueRef entry_object = entries_array[i];
        if(!entry_object.isObject()) {
            invalid_objects_ids.push_back(i);
            continue;
        }
        auto question_value = entry_object["question"];
        auto response_value = entry_object["response"];
        auto is_checked_question_value = entry_object["isCheckedQuestion"];
        auto is_checked_response_value = entry_object["isCheckedResponse"];

        if(!question_value.isString() || !response_value.isString() || !is_checked_question_value.isBool() || !is_checked_response_value.isBool()) {
            invalid_objects_ids.push_back(i);
            continue;
        }
        // Here's the filter
        if(is_checked_response_value != is_checked) continue;

        auto question = question_value.toString();
        auto response = response_value.toString();
        auto is_checked_question = is_checked_question_value.toBool();
        auto is_checked_response = is_checked_response_value.toBool();

        all_entries.emplace_back(id, question, response, is_checked_question, is_checked_response);
    }
    // Checking if any problem occured
    if(!invalid_objects_ids.empty()) {
        throw ObjectInvalidJsonException("Some JSON objects are not valid!", std::move(invalid_objects_ids));
    }
    return all_entries;
}

void DictJsonRepo::insert_entry(const QuestionResponseEntry &entry)
{
    std::list<QuestionResponseEntry> entries = {entry};
    insert_multiple_entries(entries);
}

void DictJsonRepo::insert_multiple_entries(const std::list<QuestionResponseEntry> &entries)
{
    if(m_json_document.isNull()) {
        throw FileInvalidJsonException("The JSON Document is invalid!");
    }
    if(!m_json_document.isArray()) {
        throw FileInvalidJsonException("Invalid syntax of JSON!");
    }
    auto entries_array = m_json_document.array();

    for(const auto& entry : entries) {
        QJsonObject object;
        auto entry_map = entry.getMap();

        object["question"] = entry_map["question"];
        object["response"] = entry_map["response"];
        object["isCheckedQuestion"] = entry_map["isCheckedQuestion"];
        object["isCheckedResponse"] = entry_map["isCheckedResponse"];

        entries_array.append(object.value());
    }

    // TODO : do we need this?
    m_json_document.setArray(entries_array);
    save();
}

void DictJsonRepo::save() const
{
    if(m_json_document.isNull()) {
        throw FileInvalidJsonException("The JSON Document is invalid!");
    }
    if(!m_json_document.isArray()) {
        throw FileInvalidJsonException("Invalid syntax of JSON!");
    }

    QFile json_file(m_json_path);
    if(!json_file.open(QIODevice::WriteOnly)) {
        throw FileInvalidJsonException("Cannot open the file " + m_json_path);
    }

    QTextStream file_stream(&json_file);
    file_stream << m_json_document.toJson();
}

