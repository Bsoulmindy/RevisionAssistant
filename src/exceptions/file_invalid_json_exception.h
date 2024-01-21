#ifndef FILEINVALIDJSONEXCEPTION_H
#define FILEINVALIDJSONEXCEPTION_H

#include "json_exception.h"

class FileInvalidJsonException : public JsonException
{
public:
    FileInvalidJsonException(std::string message);
};

#endif // FILEINVALIDJSONEXCEPTION_H
