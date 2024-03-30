#include "fileexception.h"


FileException::FileException(QString message) : m_message(message.toStdString()) {}

const char *FileException::what() const noexcept
{
    return m_message.c_str();
}
