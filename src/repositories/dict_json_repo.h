#ifndef DICTJSONREPO_H
#define DICTJSONREPO_H

#include "dict_repo_interface.h"
#include <QString>
#include <QJsonDocument>
#include "../models/dict_mode_enum.h"

/*
Here's an example of the structured JSON should be like
"mode" : "ManyToMany",
"questions" : [
    {
        "question": "q1",
        "response": "q2",
        "isCheckedQuestion": false,
        "isCheckedResponse": false
    },
    ...
],
"responses" : [
    ...
]
the id of each entry is its index in the array
*/
class DictJsonRepo : public DictRepoInterface
{
public:
    DictJsonRepo(QString json_path);

    std::list<QuestionResponseEntry> select_all() override;
    // TODO : need tests
    std::list<QuestionResponseEntry> select_all_questions() override;
    std::list<QuestionResponseEntry> select_all_responses() override;
    bool is_valid_question_id(int id) override;
    bool is_valid_response_id(int id) override;

    void update_question(int id, bool is_checked) override;
    void update_response(int id, bool is_checked) override;
    void mark_all_entries_unchecked() override;
    void delete_all() override;
    std::list<QuestionResponseEntry> select_questions(bool is_checked) override;
    std::list<QuestionResponseEntry> select_responses(bool is_checked) override;
    // OneToOne
    void insert_entry(const QuestionResponseEntry& entry) override;
    void insert_multiple_entries(const std::list<QuestionResponseEntry>& entries) override;
    // ManyToMany
    void insert_question(const QuestionResponseEntry& question) override;
    void insert_response(const QuestionResponseEntry& response) override;
    void insert_multiple_entries_MToM(const std::list<QuestionResponseEntry>& questions, const std::list<QuestionResponseEntry>& responses) override;
    QuestionResponseEntry select_by_id(int id) override;
    QString get_file_name() override;
    QByteArray get_byte_array() override;
    void delete_by_id(int id) override;
    void edit_entry(int id, const QString& question, const QString& response) override;
    // TODO : need tests
    DictModeEnum get_mode() override;
private:
    // Save the changes to the json file
    // Called every time when a change has been made to the document
    void save() const;

    // By default, the OneToOne mode will be selected
    void create_empty_json_file(QString json_path) const;
    /**
     * @brief Reads the json document if it is in a good format, tries to correct it if it is in old format,
     * otherwise return exception.
     */
    QJsonObject check_json_format();
    // true if it is possible to switch mode
    bool switch_mode(DictModeEnum mode);

    QString m_json_path;
    QJsonDocument m_json_document;
};

#endif // DICTJSONREPO_H
