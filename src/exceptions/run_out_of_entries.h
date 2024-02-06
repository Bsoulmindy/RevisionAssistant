#ifndef RUNOUTOFENTRIES_H
#define RUNOUTOFENTRIES_H

#include <exception>
#include <string>
#include <QString>

class RunOutOfEntries : public std::exception
{
public:
    RunOutOfEntries(QString message);
    const char *what() const noexcept override;
private:
    std::string m_message;
};

#endif // RUNOUTOFENTRIES_H
