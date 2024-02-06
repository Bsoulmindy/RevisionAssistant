#ifndef JSONEXCEPTION_H
#define JSONEXCEPTION_H

#include <QString>
#include "repo_exception.h"

class JsonException : public RepoException
{
public:
    JsonException(QString message);
    const char *what() const noexcept override;
private:
    std::string m_message;
};

#endif // JSONEXCEPTION_H
