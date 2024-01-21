#include "object_invalid_json_exception.h"

ObjectInvalidJsonException::ObjectInvalidJsonException(std::string message, std::list<int> corrupt_ids) noexcept
    : m_message(message),
    m_corrupt_ids(corrupt_ids)
{}

ObjectInvalidJsonException::ObjectInvalidJsonException(std::string message, std::list<int> &&corrupt_ids) noexcept
    : m_message(message),
    m_corrupt_ids(std::move(corrupt_ids))
{}

std::list<int> ObjectInvalidJsonException::get_corrupt_ids() const noexcept
{
    return m_corrupt_ids;
}
