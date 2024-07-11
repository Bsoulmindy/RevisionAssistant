#include "dict_json_repo.h"
#include <QFile>
#include "../exceptions/json_exception.h"
#include "../exceptions/file_invalid_json_exception.h"
#include "../models/question_response_entry.h"
#include "../exceptions/object_invalid_json_exception.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QDir>
#include <QSettings>
#include "../utils/filesystemutils.h"

DictJsonRepo::DictJsonRepo(QString json_path)
{
    QByteArray bytes;
    m_json_path = json_path;
    QFile json_file(json_path);
    if(!json_file.open(QIODevice::ReadOnly)) {
        // Attempt auto fix
        create_empty_json_file(json_path);

        if(!json_file.open(QIODevice::ReadOnly))
            throw FileInvalidJsonException("Cannot open the file " + json_path);
    }
    bytes = json_file.readAll();

    QJsonParseError json_error;
    m_json_document = QJsonDocument::fromJson(bytes, &json_error);
    if(json_error.error != QJsonParseError::NoError) {
        // Attempt auto fix : by erasing everything
        create_empty_json_file(json_path);
        qWarning() << "Error occured when reading the file " + json_path + " as JSON : " + json_error.errorString();

        m_json_document = QJsonDocument::fromJson(bytes, &json_error);
        if(json_error.error != QJsonParseError::NoError)
            throw FileInvalidJsonException("Error occured when reading the file " + json_path + " as JSON : " + json_error.errorString());
    }
}

std::list<QuestionResponseEntry> DictJsonRepo::select_all()
{
    std::list<QuestionResponseEntry> all_entries;
    QJsonObject object = check_json_format();
    QJsonArray entries_array;
    if(object["mode"].toString() == "OneToOne") {
        entries_array = object["data"].toArray();
    } else if(object["mode"].toString() == "ManyToMany") {
        // Append both questions and responses
        entries_array = object["questions"].toArray();
        auto array_to_append = object["responses"].toArray();
        for(const auto& val : array_to_append) {
            entries_array.append(val);
        }
    }

    for(int i = 0; i < entries_array.size(); i++) {
        QJsonValue entry_object = entries_array[i];
        if(!entry_object.isObject()) {
            continue;
        }
        auto question_value = entry_object["question"];
        auto response_value = entry_object["response"];
        auto is_checked_question_value = entry_object["isCheckedQuestion"];
        auto is_checked_response_value = entry_object["isCheckedResponse"];

        if(!question_value.isString() || !response_value.isString() || !is_checked_question_value.isBool() || !is_checked_response_value.isBool()) {
            continue;
        }
        auto question = question_value.toString();
        auto response = response_value.toString();
        auto is_checked_question = is_checked_question_value.toBool();
        auto is_checked_response = is_checked_response_value.toBool();

        all_entries.emplace_back(i, question, response, is_checked_question, is_checked_response);
    }
    return all_entries;
}

std::list<QuestionResponseEntry> DictJsonRepo::select_all_questions()
{
    std::list<QuestionResponseEntry> all_entries;
    QJsonObject object = check_json_format();
    QJsonArray entries_array;
    if(object["mode"].toString() == "OneToOne") {
        entries_array = object["data"].toArray();
    } else if(object["mode"].toString() == "ManyToMany") {
        // Append questions
        entries_array = object["questions"].toArray();
    }

    for(int i = 0; i < entries_array.size(); i++) {
        QJsonValue entry_object = entries_array[i];
        if(!entry_object.isObject()) {
            continue;
        }
        auto question_value = entry_object["question"];
        auto response_value = entry_object["response"];
        auto is_checked_question_value = entry_object["isCheckedQuestion"];
        auto is_checked_response_value = entry_object["isCheckedResponse"];

        if(!question_value.isString() || !response_value.isString() || !is_checked_question_value.isBool() || !is_checked_response_value.isBool()) {
            continue;
        }
        auto question = question_value.toString();
        auto response = response_value.toString();
        auto is_checked_question = is_checked_question_value.toBool();
        auto is_checked_response = is_checked_response_value.toBool();

        all_entries.emplace_back(i, question, response, is_checked_question, is_checked_response);
    }
    return all_entries;
}

std::list<QuestionResponseEntry> DictJsonRepo::select_all_responses()
{
    std::list<QuestionResponseEntry> all_entries;
    QJsonObject object = check_json_format();
    QJsonArray entries_array;
    int index_offset = 0;
    if(object["mode"].toString() == "OneToOne") {
        entries_array = object["data"].toArray();
    } else if(object["mode"].toString() == "ManyToMany") {
        // Append responses
        index_offset = object["questions"].toArray().size();
        entries_array = object["responses"].toArray();
    }

    for(int i = 0; i < entries_array.size(); i++) {
        QJsonValue entry_object = entries_array[i];
        if(!entry_object.isObject()) {
            continue;
        }
        auto question_value = entry_object["question"];
        auto response_value = entry_object["response"];
        auto is_checked_question_value = entry_object["isCheckedQuestion"];
        auto is_checked_response_value = entry_object["isCheckedResponse"];

        if(!question_value.isString() || !response_value.isString() || !is_checked_question_value.isBool() || !is_checked_response_value.isBool()) {
            continue;
        }
        auto question = question_value.toString();
        auto response = response_value.toString();
        auto is_checked_question = is_checked_question_value.toBool();
        auto is_checked_response = is_checked_response_value.toBool();

        all_entries.emplace_back(i + index_offset, question, response, is_checked_question, is_checked_response);
    }
    return all_entries;
}

void DictJsonRepo::update_question(int id, bool is_checked)
{
    QJsonObject object = check_json_format();
    QJsonArray entries_array;
    if(object["mode"].toString() == "OneToOne") {
        entries_array = object["data"].toArray();
    } else if(object["mode"].toString() == "ManyToMany") {
        entries_array = object["questions"].toArray();
    }
    if(id >= entries_array.size()) {
        throw JsonException("ID " + QString::number(id) + " doesn't exist in JSON");
    }
    auto entry_object = entries_array[id].toObject();
    if(!entry_object["isCheckedQuestion"].isBool()) {
        throw ObjectInvalidJsonException("Invalid JSON Object ID " + QString::number(id), {id});
    }
    entry_object["isCheckedQuestion"] = is_checked;
    entries_array[id] = entry_object;
    if(object["mode"].toString() == "OneToOne") {
        object["data"] = entries_array;
    } else if(object["mode"].toString() == "ManyToMany") {
        object["questions"] = entries_array;
    }
    m_json_document.setObject(object);

    save();
}

void DictJsonRepo::update_response(int id, bool is_checked)
{
    QJsonObject object = check_json_format();
    QJsonArray entries_array;
    int size_questions = -1;
    if(object["mode"].toString() == "OneToOne") {
        entries_array = object["data"].toArray();
    } else if(object["mode"].toString() == "ManyToMany") {
        // Append responses only
        entries_array = object["responses"].toArray();
        size_questions = object["questions"].toArray().size();
        // correspond id to responses array
        if(id < size_questions) {
            throw JsonException("ID " + QString::number(id) + " doesn't exist in JSON");
        }
        id -= size_questions;
    }

    if(id >= entries_array.size()) {
        throw JsonException("ID " + QString::number(id) + " doesn't exist in JSON");
    }
    auto entry_object = entries_array[id].toObject();
    if(!entry_object["isCheckedResponse"].isBool()) {
        throw ObjectInvalidJsonException("Invalid JSON Object ID " + QString::number(id), {id});
    }
    entry_object["isCheckedResponse"] = is_checked;
    entries_array[id] = entry_object;
    if(object["mode"].toString() == "OneToOne") {
        object["data"] = entries_array;
    } else if(object["mode"].toString() == "ManyToMany") {
        object["responses"] = entries_array;
    }
    m_json_document.setObject(object);

    save();
}

void DictJsonRepo::mark_all_entries_unchecked()
{
    QJsonObject object = check_json_format();
    QJsonArray entries_array; // data or questions
    QJsonArray response_array;
    if(object["mode"].toString() == "OneToOne") {
        entries_array = object["data"].toArray();
    } else if(object["mode"].toString() == "ManyToMany") {
        // Append both questions and responses
        entries_array = object["questions"].toArray();
        response_array = object["responses"].toArray();
    }
    // Do the callback for both arrays
    auto callback = [&](QJsonArray& arr)
    {
        for(int i = 0; i < arr.size(); i++) {
            auto entry_object = arr[i].toObject();

            if(!entry_object["isCheckedResponse"].isBool() || !entry_object["isCheckedQuestion"].isBool()) {
                throw ObjectInvalidJsonException("Invalid JSON Object ID " + QString::number(i), {i});
            }

            entry_object["isCheckedResponse"] = false;
            entry_object["isCheckedQuestion"] = false;

            arr[i] = entry_object;
        }
    };
    callback(entries_array);
    callback(response_array);
    if(object["mode"].toString() == "OneToOne") {
        object["data"] = entries_array;
    } else if(object["mode"].toString() == "ManyToMany") {
        object["questions"] = entries_array;
        object["responses"] = response_array;
    }
    m_json_document.setObject(object);

    save();
}

void DictJsonRepo::delete_all()
{
    QJsonObject object = check_json_format();
    if(object["mode"].toString() == "OneToOne") {
        object["data"] = QJsonArray();
    } else if(object["mode"].toString() == "ManyToMany") {
        object["questions"] = QJsonArray();
        object["responses"] = QJsonArray();
    }

    m_json_document.setObject(object);

    save();
}

std::list<QuestionResponseEntry> DictJsonRepo::select_questions(bool is_checked)
{
    QJsonObject object = check_json_format();
    QJsonArray entries_array;
    if(object["mode"].toString() == "OneToOne") {
        entries_array = object["data"].toArray();
    } else if(object["mode"].toString() == "ManyToMany") {
        entries_array = object["questions"].toArray();
    }
    std::list<QuestionResponseEntry> all_entries;

    for(int i = 0; i < entries_array.size(); i++) {
        if(!entries_array[i].isObject()) {
            continue;
        }
        auto entry_object = entries_array[i].toObject();

        auto question_value = entry_object["question"];
        auto response_value = entry_object["response"];
        auto is_checked_question_value = entry_object["isCheckedQuestion"];
        auto is_checked_response_value = entry_object["isCheckedResponse"];

        if(!question_value.isString() || !response_value.isString() || !is_checked_question_value.isBool() || !is_checked_response_value.isBool()) {
            continue;
        }
        // Here's the filter
        if(is_checked_question_value != is_checked) continue;

        auto question = question_value.toString();
        auto response = response_value.toString();
        auto is_checked_question = is_checked_question_value.toBool();
        auto is_checked_response = is_checked_response_value.toBool();

        all_entries.emplace_back(i, question, response, is_checked_question, is_checked_response);
    }
    return all_entries;
}

std::list<QuestionResponseEntry> DictJsonRepo::select_responses(bool is_checked)
{
    QJsonObject object = check_json_format();
    QJsonArray entries_array;
    int id_offset = 0;
    if(object["mode"].toString() == "OneToOne") {
        entries_array = object["data"].toArray();
    } else if(object["mode"].toString() == "ManyToMany") {
        entries_array = object["responses"].toArray();
        id_offset = object["questions"].toArray().size();
    }
    std::list<QuestionResponseEntry> all_entries;

    for(int i = 0; i < entries_array.size(); i++) {
        if(!entries_array[i].isObject()) {
            continue;
        }
        auto entry_object = entries_array[i].toObject();

        auto question_value = entry_object["question"];
        auto response_value = entry_object["response"];
        auto is_checked_question_value = entry_object["isCheckedQuestion"];
        auto is_checked_response_value = entry_object["isCheckedResponse"];

        if(!question_value.isString() || !response_value.isString() || !is_checked_question_value.isBool() || !is_checked_response_value.isBool()) {
            continue;
        }
        // Here's the filter
        if(is_checked_response_value != is_checked) continue;

        auto question = question_value.toString();
        auto response = response_value.toString();
        auto is_checked_question = is_checked_question_value.toBool();
        auto is_checked_response = is_checked_response_value.toBool();

        all_entries.emplace_back(i + id_offset, question, response, is_checked_question, is_checked_response);
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
    QJsonObject object = check_json_format();
    QJsonArray entries_array;
    if(object["mode"].toString() != "OneToOne" && !switch_mode(DictModeEnum::OneToOne)) {
        throw FileInvalidJsonException("The dict is not in OneToOne mode. It is in " + object["mode"].toString());
    }
    entries_array = object["data"].toArray();

    for(const auto& entry : entries) {
        QJsonObject object;
        auto entry_map = entry.getMap();

        object["question"] = entry_map["question"].toString();
        object["response"] = entry_map["response"].toString();
        object["isCheckedQuestion"] = entry_map["isCheckedQuestion"].toBool();
        object["isCheckedResponse"] = entry_map["isCheckedResponse"].toBool();

        entries_array.append(object);
    }

    object["data"] = entries_array;
    m_json_document.setObject(object);
    save();
}

void DictJsonRepo::insert_question(const QuestionResponseEntry &question)
{
    std::list<QuestionResponseEntry> questions = {question};
    std::list<QuestionResponseEntry> responses;
    insert_multiple_entries_MToM(questions, responses);
}

void DictJsonRepo::insert_response(const QuestionResponseEntry &response)
{
    std::list<QuestionResponseEntry> questions;
    std::list<QuestionResponseEntry> responses = {response};
    insert_multiple_entries_MToM(questions, responses);
}

void DictJsonRepo::insert_multiple_entries_MToM(const std::list<QuestionResponseEntry> &questions, const std::list<QuestionResponseEntry> &responses)
{
    QJsonObject object = check_json_format();
    QJsonArray questions_array;
    QJsonArray responses_array;
    if(object["mode"].toString() != "ManyToMany" && !switch_mode(DictModeEnum::ManyToMany)) {
        throw FileInvalidJsonException("The dict is not in ManyToMany mode. It is in " + object["mode"].toString());
    }
    object = check_json_format();
    questions_array = object["questions"].toArray();
    responses_array = object["responses"].toArray();

    for(const auto& question : questions) {
        QJsonObject entry_object;
        auto entry_map = question.getMap();

        entry_object["question"] = entry_map["question"].toString();
        entry_object["response"] = entry_map["response"].toString();
        entry_object["isCheckedQuestion"] = entry_map["isCheckedQuestion"].toBool();
        entry_object["isCheckedResponse"] = entry_map["isCheckedResponse"].toBool();

        questions_array.append(entry_object);
    }

    for(const auto& response : responses) {
        QJsonObject entry_object;
        auto entry_map = response.getMap();

        entry_object["question"] = entry_map["question"].toString();
        entry_object["response"] = entry_map["response"].toString();
        entry_object["isCheckedQuestion"] = entry_map["isCheckedQuestion"].toBool();
        entry_object["isCheckedResponse"] = entry_map["isCheckedResponse"].toBool();

        responses_array.append(entry_object);
    }

    object["questions"] = questions_array;
    object["responses"] = responses_array;
    m_json_document.setObject(object);
    save();
}

QuestionResponseEntry DictJsonRepo::select_by_id(int id)
{
    QJsonObject object = check_json_format();
    QJsonArray entries_array;
    if(object["mode"].toString() == "OneToOne") {
        entries_array = object["data"].toArray();
    } else if(object["mode"].toString() == "ManyToMany") {
        // Append both questions and responses
        entries_array = object["questions"].toArray();
        auto array_to_append = object["responses"].toArray();
        for(const auto& val : array_to_append) {
            entries_array.append(val);
        }
    }

    if(id >= entries_array.size()) {
        throw JsonException("Json Object ID " + QString::number(id) + " not found!");
    }

    auto entry_object = entries_array[id].toObject();

    auto question_value = entry_object["question"];
    auto response_value = entry_object["response"];
    auto is_checked_question_value = entry_object["isCheckedQuestion"];
    auto is_checked_response_value = entry_object["isCheckedResponse"];

    if(!question_value.isString() || !response_value.isString() || !is_checked_question_value.isBool() || !is_checked_response_value.isBool()) {
        throw ObjectInvalidJsonException("Invalid Object ID " + QString::number(id), {id});
    }

    auto question = question_value.toString();
    auto response = response_value.toString();
    auto is_checked_question = is_checked_question_value.toBool();
    auto is_checked_response = is_checked_response_value.toBool();

    return QuestionResponseEntry(id, question, response, is_checked_question, is_checked_response);
}

QString DictJsonRepo::get_file_name()
{
    return m_json_path;
}

QByteArray DictJsonRepo::get_byte_array()
{
    return m_json_document.toJson(QJsonDocument::Compact);
}

void DictJsonRepo::delete_by_id(int id)
{
    QJsonObject object = check_json_format();
    QJsonArray entries_array;
    QString key = "";
    if(object["mode"].toString() == "OneToOne") {
        entries_array = object["data"].toArray();
        key = "data";
    } else if(object["mode"].toString() == "ManyToMany") {
        entries_array = object["questions"].toArray();
        // Are we talking about a Response?
        if(id >= entries_array.size()) {
            id -= entries_array.size();
            entries_array = object["responses"].toArray();
            key = "responses";
        } else { // Or question?
            key = "questions";
        }
    }
    if(id >= entries_array.size() || id < 0) {
        throw JsonException("ID " + QString::number(id) + " doesn't exist in JSON");
    }

    entries_array.removeAt(id);
    object[key] = entries_array;

    m_json_document.setObject(object);
    save();
}

void DictJsonRepo::edit_entry(int id, const QString& question, const QString& response)
{
    QJsonObject object = check_json_format();
    QJsonArray entries_array;
    QString key = "";
    if(object["mode"].toString() == "OneToOne") {
        entries_array = object["data"].toArray();
        key = "data";
    } else if(object["mode"].toString() == "ManyToMany") {
        entries_array = object["questions"].toArray();
        // Are we talking about a Response?
        if(id >= entries_array.size()) {
            id -= entries_array.size();
            entries_array = object["responses"].toArray();
            key = "responses";
        } else { // Or question?
            key = "questions";
        }
    }

    if(id >= entries_array.size()) {
        throw JsonException("ID " + QString::number(id) + " doesn't exist in JSON");
    }
    auto entry_object = entries_array[id].toObject();
    if(!entry_object["question"].isString() || !entry_object["response"].isString()) {
        throw ObjectInvalidJsonException("Invalid JSON Object ID " + QString::number(id), {id});
    }
    entry_object["question"] = question;
    entry_object["response"] = response;
    entries_array[id] = entry_object;
    object[key] = entries_array;
    m_json_document.setObject(object);

    save();
}

DictModeEnum DictJsonRepo::get_mode()
{
    QJsonObject object = check_json_format();
    if(object["mode"].toString() == "OneToOne") {
        return OneToOne;
    } else if(object["mode"].toString() == "ManyToMany") {
        return ManyToMany;
    }
    return Unknown;
}

void DictJsonRepo::save() const
{
    if(m_json_document.isNull() || !m_json_document.isObject()) {
        throw FileInvalidJsonException("The JSON Document is invalid!");
    }
    QFile json_file(m_json_path);
    if(!json_file.open(QIODevice::WriteOnly)) {
        // Attempt auto fix
        create_empty_json_file(m_json_path);

        if(!json_file.open(QIODevice::WriteOnly))
            throw FileInvalidJsonException("Cannot open the file " + m_json_path);
    }

    QTextStream file_stream(&json_file);
    file_stream << m_json_document.toJson(QJsonDocument::Compact);

    FileSystemUtils::save_changes_to_storage();
}

void DictJsonRepo::create_empty_json_file(QString json_path) const
{
    QFile json_file(json_path);
    auto dirs = json_path.split("/");
    QString dir = "";
    for(int i = 0; i < dirs.size() - 1; i++) {
        dir += dirs[i] + "/";
    }
    QDir qDir;
    if(!qDir.mkpath(dir)) {
        qCritical() << "Failed to create the directory " + dir;
    }

    if(!json_file.open(QIODevice::ReadWrite)) {
        qCritical() << "Failed to open the file : " + json_file.errorString();
    }

    // OneToOne mode
    QJsonDocument empty_doc;
    QJsonObject object;
    object["mode"] = QString("OneToOne");
    object["data"] = QJsonArray();
    empty_doc.setObject(object);
    auto bytes = empty_doc.toJson(QJsonDocument::Compact);

    QTextStream text_stream(&json_file);
    text_stream << bytes;

    FileSystemUtils::save_changes_to_storage();
}

QJsonObject DictJsonRepo::check_json_format()
{
    if(m_json_document.isNull()) {
        throw FileInvalidJsonException("The JSON Document is invalid!");
    }
    // Old format : convert to OneToOne association
    if(m_json_document.isArray()) {
        auto array = m_json_document.array();
        QJsonObject object;
        object["mode"] = QString("OneToOne");
        object["data"] = array;

        m_json_document.setObject(object);
        save();
    }

    // Checks if the JSON is valid
    if(!m_json_document.isObject()) {
        throw FileInvalidJsonException("Invalid syntax of JSON!");
    }
    auto object = m_json_document.object();
    if(!object.contains("mode")) {
        throw FileInvalidJsonException("Invalid syntax of JSON : inexistant mode!");
    }
    QString mode = object["mode"].toString();
    if(mode == "OneToOne" && !object.contains("data")) {
        object["data"] = QJsonArray();
        m_json_document.setObject(object);
        save();
    }
    if(mode == "ManyToMany" && (!object.contains("questions") || !object.contains("responses"))) {
        if(!object.contains("questions")) {
            object["questions"] = QJsonArray();
        }
        if(!object.contains("responses")) {
            object["responses"] = QJsonArray();
        }

        m_json_document.setObject(object);
        save();
    }
    if(mode != "OneToOne" && mode != "ManyToMany") {
        throw FileInvalidJsonException("Invalid syntax of JSON : unknown mode!");
    }

    return object;
}

bool DictJsonRepo::switch_mode(DictModeEnum mode)
{
    QJsonObject object = check_json_format();
    switch(mode) {
    case OneToOne:
        if(object["mode"] == "OneToOne") return true;
        if(object["mode"] == "ManyToMany" && object["questions"].toArray().size() == 0 && object["responses"].toArray().size() == 0) {
            object["mode"] = "OneToOne";
            object.remove("questions");
            object.remove("responses");
            object["data"] = QJsonArray();
            m_json_document.setObject(object);
            save();
            return true;
        }
        return false;
    case ManyToMany:
        if(object["mode"] == "ManyToMany") return true;
        if(object["mode"] == "OneToOne" && object["data"].toArray().size() == 0) {
            object["mode"] = "ManyToMany";
            object.remove("data");
            object["questions"] = QJsonArray();
            object["responses"] = QJsonArray();
            m_json_document.setObject(object);
            save();
            return true;
        }
        return false;
    }

    return false;
}


