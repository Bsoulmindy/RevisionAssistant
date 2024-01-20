#ifndef DICTJSONREPO_H
#define DICTJSONREPO_H

#include "dict_repo_interface.h"
#include <QString>
#include <QJsonDocument>

/*
Here's an exampel of the structured JSON should be like
[
    {
        "question": "q1",
        "response": "q2",
        "isCheckedQuestion": false,
        "isCheckedResponse": false
    },
    {
        "question": "q1",
        "response": "q2",
        "isCheckedQuestion": false,
        "isCheckedResponse": false
    },
    {
        "question": "q1",
        "response": "q2",
        "isCheckedQuestion": false,
        "isCheckedResponse": false
    }
]
the id of each entry is its index in the array
*/
class DictJsonRepo : public DictRepoInterface
{
public:
    DictJsonRepo(QString json_path);

    std::list<QuestionResponseEntry> select_all();
    void update_question(int id, bool is_checked);
    void update_response(int id, bool is_checked);
    void mark_all_entries_unchecked();
    void delete_all();
    std::list<QuestionResponseEntry> select_questions(bool is_checked);
    std::list<QuestionResponseEntry> select_responses(bool is_checked);
    void insert_entry(const QuestionResponseEntry& entry);

private:
    QString m_json_path;
    QJsonDocument m_json_document;
};

#endif // DICTJSONREPO_H
