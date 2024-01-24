#include "json_exception.h"

JsonException::JsonException(QString message) : m_message(message.toStdString()) {}

const char *JsonException::what() noexcept
{
    return m_message.c_str();
}

