#include "dict_json_repo.h"
#include <QFile>
#include "../exceptions/json_exception.h"

DictJsonRepo::DictJsonRepo(QString json_path)
{
    m_json_path = json_path;
    QFile json_file(json_path);
    if(!json_file.open(QIODevice::ReadOnly)) {
        throw JsonException("Cannot open the file " + json_path);
    }

    auto bytes = json_file.readAll();
    QJsonParseError json_error;
    m_json_document = QJsonDocument::fromJson(bytes, &json_error);
    if(json_error != QJsonParseError::NoError) {
        throw JsonException("Error occured when reading the file " + json_path + " as JSON : " + json_error.errorString());
    }
}

std::list<QuestionResponseEntry> DictJsonRepo::select_all()
{

}
