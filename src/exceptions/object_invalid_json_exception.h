#ifndef OBJECTINVALIDJSONEXCEPTION_H
#define OBJECTINVALIDJSONEXCEPTION_H

#include "json_exception.h"

class ObjectInvalidJsonException : public JsonException
{
public:
    ObjectInvalidJsonException(std::string message, std::list<int> corrupt_ids) noexcept;
    ObjectInvalidJsonException(std::string message, std::list<int>&& corrupt_ids) noexcept;

    std::list<int> get_corrupt_ids() const noexcept;
private:
    std::list<int> m_corrupt_ids;
};

#endif // OBJECTINVALIDJSONEXCEPTION_H
