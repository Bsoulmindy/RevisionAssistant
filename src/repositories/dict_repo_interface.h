#ifndef DICTREPOINTERFACE_H
#define DICTREPOINTERFACE_H

#include "../models/question_response_entry.h"
#include <list>

class DictRepoInterface
{
public:
    virtual ~DictRepoInterface();
    virtual std::list<QuestionResponseEntry> select_all() = 0;
    virtual void update_question(int id, bool is_checked) = 0;
    virtual void update_response(int id, bool is_checked) = 0;
    virtual void mark_all_entries_unchecked() = 0;
    virtual void delete_all() = 0;
    virtual std::list<QuestionResponseEntry> select_questions(bool is_checked) = 0;
    virtual std::list<QuestionResponseEntry> select_responses(bool is_checked) = 0;
    virtual void insert_entry(const QuestionResponseEntry& entry) = 0;
};

#endif // DICTREPOINTERFACE_H
