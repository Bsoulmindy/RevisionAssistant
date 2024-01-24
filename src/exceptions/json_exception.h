#ifndef JSONEXCEPTION_H
#define JSONEXCEPTION_H

#include <QString>

class JsonException : public std::exception
{
public:
    JsonException(QString message);
    const char *what() noexcept;
private:
    std::string m_message;
};

#endif // JSONEXCEPTION_H
