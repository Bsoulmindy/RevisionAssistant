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
        auto entry_object = entry_object.toObject();
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
