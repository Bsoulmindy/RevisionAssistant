#ifndef DICTREPOINTERFACE_H
#define DICTREPOINTERFACE_H

#include "../models/question_response_entry.h"
#include <vector>

class DictRepoInterface
{
public:
    virtual ~DictRepoInterface();
    virtual std::vector<QuestionResponseEntry> select_all() = 0;
};

#endif // DICTREPOINTERFACE_H
