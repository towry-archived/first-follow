//
// Find first&follow set.
//
// Copyright 2014 Towry Wang <http://towry.me>
// MIT License
//
// $2014/9/15$
//

#include "file-helper.h"

FileHelper::FileHelper()
{
    m_fbPtr = m_is.rdbuf();
}

FileHelper::~FileHelper()
{
    m_is.close();
    m_fbPtr->close();
}

void FileHelper::in(const char *fn)
{
    if (m_fbPtr->is_open())
    {
        m_fbPtr->close();
    }

    m_fbPtr->open(fn, std::ios::in);

    if (!m_fbPtr->is_open())
    {
        m_serror = "Failed to open file.";
        return;
    }
}

void FileHelper::out(const char *fn)
{
    if (m_fbPtr->is_open())
    {
        m_fbPtr->close();
    }
}

bool FileHelper::error()
{
    return m_serror.length() != 0;
}

std::string FileHelper::getError()
{
    return m_serror;
}

const char& FileHelper::nextc(char &c) const
{
    c = m_fbPtr->sbumpc();
    return c;
}

const char FileHelper::ungetc() const
{
    return m_fbPtr->sungetc();
}
