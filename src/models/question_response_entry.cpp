#include "question_response_entry.h"
#include <QString>
#include <QVariantMap>

QuestionResponseEntry::QuestionResponseEntry(int id,
                                             QString question,
                                             QString response,
                                             bool is_checked_question,
                                             bool is_checked_response) noexcept :
    m_id(id),
    m_question(question),
    m_response(response),
    m_is_checked_question(is_checked_question),
    m_is_checked_response(is_checked_response)
{}

QVariantMap QuestionResponseEntry::getMap() const noexcept
{
    QVariantMap map;
    map["id"] = m_id;
    map["question"] = m_question;
    map["response"] = m_response;
    map["isCheckedQuestion"] = m_is_checked_question;
    map["isCheckedResponse"] = m_is_checked_response;

    return map;
}

