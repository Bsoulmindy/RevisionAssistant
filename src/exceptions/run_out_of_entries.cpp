#include "run_out_of_entries.h"


RunOutOfEntries::RunOutOfEntries(QString message) : m_message(message.toStdString()) {}

const char *RunOutOfEntries::what() const noexcept
{
    return m_message.c_str();
}
