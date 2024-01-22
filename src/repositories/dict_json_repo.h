#ifndef DICTJSONREPO_H
#define DICTJSONREPO_H

#include "dict_repo_interface.h"
#include <QString>
#include <QJsonDocument>

/*
Here's an example of the structured JSON should be like
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

    std::list<QuestionResponseEntry> select_all() override;
    void update_question(int id, bool is_checked) override;
    void update_response(int id, bool is_checked) override;
    void mark_all_entries_unchecked() override;
    void delete_all() override;
    std::list<QuestionResponseEntry> select_questions(bool is_checked) override;
    std::list<QuestionResponseEntry> select_responses(bool is_checked) override;
    void insert_entry(const QuestionResponseEntry& entry) override;
    void insert_multiple_entries(const std::list<QuestionResponseEntry>& entries) override;
private:
    // Save the changes to the json file
    // Called every time when a change has been made to the document
    void save() const;

    QString m_json_path;
    QJsonDocument m_json_document;
};

#endif // DICTJSONREPO_H
