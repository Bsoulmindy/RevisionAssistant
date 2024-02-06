#ifndef QUESTIONRESPONSEENTRIESSET_H
#define QUESTIONRESPONSEENTRIESSET_H

#include "qqml.h"
#include <unordered_set>
#include <QString>

class QuestionResponseEntriesSet
{
public:
    QuestionResponseEntriesSet(std::unordered_set<QString>&& questions_set,
                               std::unordered_set<QString>&& responses_set) noexcept;

    std::unordered_set<QString> get_questions_set() const noexcept;
    std::unordered_set<QString> get_responses_set() const noexcept;
private:
    std::unordered_set<QString> m_questions_set;
    std::unordered_set<QString> m_responses_set;
};

#endif // QUESTIONRESPONSEENTRIESSET_H
