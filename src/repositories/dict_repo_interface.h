#ifndef DICTREPOINTERFACE_H
#define DICTREPOINTERFACE_H

#include "../models/question_response_entry.h"
#include <list>

/**
 * @brief The DictRepoInterface class
 * The id should refer to an entry, independently to its location (question / response)
 */
class DictRepoInterface
{
public:
    virtual ~DictRepoInterface() {}
    virtual std::list<QuestionResponseEntry> select_all() = 0;
    virtual void update_question(int id, bool is_checked) = 0;
    virtual void update_response(int id, bool is_checked) = 0;
    virtual void mark_all_entries_unchecked() = 0;
    virtual void delete_all() = 0;
    virtual std::list<QuestionResponseEntry> select_questions(bool is_checked) = 0;
    virtual std::list<QuestionResponseEntry> select_responses(bool is_checked) = 0;
    virtual QuestionResponseEntry select_by_id(int id) = 0;
    // If you want to insert multiple entries, please use the method <code>insert_multiple_entries</code>
    // Not doing so will result in multiple saves for each insertion
    // OneToOne : Field 'Id' will be ignored
    virtual void insert_entry(const QuestionResponseEntry& entry) = 0;
    // OneToOne : more performant because it will save only once
    virtual void insert_multiple_entries(const std::list<QuestionResponseEntry>& entries) = 0;
    // ManyToMany
    virtual void insert_question(const QuestionResponseEntry& question) = 0;
    virtual void insert_response(const QuestionResponseEntry& response) = 0;
    virtual void insert_multiple_entries_MToM(const std::list<QuestionResponseEntry>& questions, const std::list<QuestionResponseEntry>& responses) = 0;
    virtual QString get_file_name() = 0;
    virtual QByteArray get_byte_array() = 0;
    /**
     * @brief
     * The implementations are required to satisfy the following requirement:
     * Given an index i that satisfy the requirement : i > id
     * Then after executing this method: i -> i - 1
     */
    virtual void delete_by_id(int id) = 0;
    virtual void edit_entry(int id, const QString& question, const QString& response) = 0;
};

#endif // DICTREPOINTERFACE_H
