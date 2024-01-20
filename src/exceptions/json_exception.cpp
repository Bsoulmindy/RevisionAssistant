#include "json_exception.h"

JsonException::JsonException(std::string message) : m_message(message) {}

const char *JsonException::what() noexcept
{
    return m_message.c_str();
}

