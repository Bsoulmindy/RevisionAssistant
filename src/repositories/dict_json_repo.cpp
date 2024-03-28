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

#if defined(Q_OS_WASM)
#include <emscripten.h>
#endif

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
    if(m_json_document.isNull()) {
        throw FileInvalidJsonException("The JSON Document is invalid!");
    }
    if(!m_json_document.isArray()) {
        throw FileInvalidJsonException("Invalid syntax of JSON!");
    }
    std::list<QuestionResponseEntry> all_entries;
    auto entries_array = m_json_document.array();
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
        throw JsonException("ID " + QString::number(id) + " doesn't exist in JSON");
    }
    auto entry_object = entries_array[id].toObject();
    if(!entry_object["isCheckedQuestion"].isBool()) {
        throw ObjectInvalidJsonException("Invalid JSON Object ID " + QString::number(id), {id});
    }
    entry_object["isCheckedQuestion"] = is_checked;
    entries_array[id] = entry_object;
    m_json_document.setArray(entries_array);

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
        throw JsonException("ID " + QString::number(id) + " doesn't exist in JSON");
    }
    auto entry_object = entries_array[id].toObject();
    if(!entry_object["isCheckedResponse"].isBool()) {
        throw ObjectInvalidJsonException("Invalid JSON Object ID " + QString::number(id), {id});
    }
    entry_object["isCheckedResponse"] = is_checked;
    entries_array[id] = entry_object;
    m_json_document.setArray(entries_array);

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
        auto entry_object = entries_array[i].toObject();

        if(!entry_object["isCheckedResponse"].isBool() || !entry_object["isCheckedQuestion"].isBool()) {
            throw ObjectInvalidJsonException("Invalid JSON Object ID " + QString::number(i), {i});
        }

        entry_object["isCheckedResponse"] = false;
        entry_object["isCheckedQuestion"] = false;

        entries_array[i] = entry_object;
    }
    m_json_document.setArray(entries_array);

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

    m_json_document.setArray(QJsonArray());

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
    std::list<QuestionResponseEntry> all_entries;
    auto entries_array = m_json_document.array();

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
    if(m_json_document.isNull()) {
        throw FileInvalidJsonException("The JSON Document is invalid!");
    }
    if(!m_json_document.isArray()) {
        throw FileInvalidJsonException("Invalid syntax of JSON!");
    }
    std::list<QuestionResponseEntry> all_entries;
    auto entries_array = m_json_document.array();

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

        all_entries.emplace_back(i, question, response, is_checked_question, is_checked_response);
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

        object["question"] = entry_map["question"].toString();
        object["response"] = entry_map["response"].toString();
        object["isCheckedQuestion"] = entry_map["isCheckedQuestion"].toBool();
        object["isCheckedResponse"] = entry_map["isCheckedResponse"].toBool();

        entries_array.append(object);
    }

    m_json_document.setArray(entries_array);
    save();
}

QuestionResponseEntry DictJsonRepo::select_by_id(int id)
{
    if(m_json_document.isNull()) {
        throw FileInvalidJsonException("The JSON Document is invalid!");
    }
    if(!m_json_document.isArray()) {
        throw FileInvalidJsonException("Invalid syntax of JSON!");
    }
    auto entries_array = m_json_document.array();

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
        // Attempt auto fix
        create_empty_json_file(m_json_path);

        if(!json_file.open(QIODevice::WriteOnly))
            throw FileInvalidJsonException("Cannot open the file " + m_json_path);
    }

    QTextStream file_stream(&json_file);
    file_stream << m_json_document.toJson();

#if defined(Q_OS_WASM)
    EM_ASM(
        FS.syncfs(false, function (err) {
            if(err)
                console.error(err);
        });
    );
#endif
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

    QJsonDocument empty_doc;
    empty_doc.setArray(QJsonArray());
    auto bytes = empty_doc.toJson();

    QTextStream text_stream(&json_file);
    text_stream << bytes;
}

