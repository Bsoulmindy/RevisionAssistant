#ifndef QUESTIONRESPONSEENTRY_H
#define QUESTIONRESPONSEENTRY_H

#include <QString>
#include <QVariantMap>

class QuestionResponseEntry
{
public:
    QuestionResponseEntry(int id, QString question, QString response, bool is_checked_question, bool is_checked_response) noexcept;
    QVariantMap getMap() const noexcept;
    QString getQuestion() const noexcept;
    QString getResponse() const noexcept;
private:
    int m_id;
    QString m_question;
    QString m_response;
    bool m_is_checked_question;
    bool m_is_checked_response;
};

#endif // QUESTIONRESPONSEENTRY_H
