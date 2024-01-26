#ifndef JSONEXCEPTION_H
#define JSONEXCEPTION_H

#include <QString>

class JsonException : public std::exception
{
public:
    JsonException(QString message);
    const char *what() const noexcept override;
private:
    std::string m_message;
};

#endif // JSONEXCEPTION_H
