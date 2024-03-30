#ifndef FILEEXCEPTION_H
#define FILEEXCEPTION_H

#include <exception>
#include <QString>

class FileException : public std::exception
{
public:
    FileException(QString message);
    const char *what() const noexcept override;
private:
    std::string m_message;
};

#endif // FILEEXCEPTION_H
