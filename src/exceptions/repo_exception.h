#ifndef REPOEXCEPTION_H
#define REPOEXCEPTION_H

#include <exception>

class RepoException : public std::exception
{
public:
    virtual ~RepoException() {}
};

#endif // REPOEXCEPTION_H
