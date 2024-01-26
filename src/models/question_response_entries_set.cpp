#include "question_response_entries_set.h"

QuestionResponseEntriesSet::QuestionResponseEntriesSet(std::unordered_set<QString> &&questions_set,
                                                       std::unordered_set<QString> &&responses_set) noexcept :
    m_questions_set(std::move(questions_set)),
    m_responses_set(std::move(responses_set)) {}

std::unordered_set<QString> QuestionResponseEntriesSet::get_questions_set() const noexcept
{
    return m_questions_set;
}

std::unordered_set<QString> QuestionResponseEntriesSet::get_responses_set() const noexcept
{
    return m_responses_set;
}

