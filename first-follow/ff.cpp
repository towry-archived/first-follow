//
// Find first&follow set.
//
// Copyright 2014 Towry Wang <http://towry.me>
// License under MIT License.
//
// $2014/9/15$
//

#include "ff.h"
#include <string> // cout string
#include <cstring>
#include <iostream>
#include <algorithm>

FF::FF()
{
    m_fhPtr = new FileHelper();
    m_config[CONFIG_ASSIGN] = "->";
}

FF::~FF()
{
    delete m_fhPtr;
}

void FF::input(const char *fn)
{
    m_fhPtr->in(fn);
    if (m_fhPtr->error())
    {
        m_err = true;
        m_message = m_fhPtr->getError();
        return;
    }

    parse();
}

void FF::output(const char *fn)
{
    std::vector<std::string> var_first;
    std::vector<std::string> var_follow;

    // first follow
    for (auto its = m_nts.begin(); its != m_nts.end(); its++)
    {
        var_first.clear();

        var_first.push_back("{ ");
        first(*its, var_first);
        var_first.push_back("}\n");

        std::cout << " first(" << *its << ") = ";
        for (auto its_inner = var_first.begin(); its_inner != var_first.end(); its_inner++)
        {
            std::cout << *its_inner;
        }
    }

    std::cout << "======================================\n";

    for (auto its = m_nts.begin(); its != m_nts.end(); its++)
    {
        var_follow.clear();

        var_follow.push_back("{");
        follow(*its, var_follow);
        var_first.push_back("}\n");

        std::cout << "follow(" << *its << ") = ";
        for (auto its_inner = var_follow.begin(); its_inner != var_follow.end(); its_inner++)
        {
            std::cout << *its_inner << " ";
        }
    }
}

void FF::parse()
{
    std::string token;
    int index = 0;
    std::vector<std::string>::iterator it;

    while (1)
    {
        token = nextToken();

        if (token == "$$")
        {
            break;
        }

        // sanity check.
        if (m_defs.empty())
        {
            std::vector<std::string> tmp_s;
            m_defs.push_back(tmp_s);
        }

        // if the token is a end symbol
        // a new def is start.
        if (token == "$")
        {
            index++;
            std::vector<std::string> tmp_s;
            m_defs.push_back(tmp_s);
            continue;
        }

        if (m_nts.size() == m_nts.max_size())
        {
            m_nts.reserve(10);
        }

        if (m_defs.size() == m_defs.max_size())
        {
            m_defs.reserve(10);
        }

        if (strcmp(token.c_str(), config(CONFIG_ASSIGN)) == 0)
        {
            if (m_nts.size() == (index + 1) && \
                m_defs[index].size() > 1)
            {
                // error! there is already a non terminal
                // but we get a assignment symbol again.
                m_err = true;
                m_message = "Bad syntax!";
                return;
            }
            else
            {
                it = m_defs[index].begin();
                m_nts.push_back(*it);
                continue;
            }
        }
        // or just put the token into m_defs.
        m_defs[index].push_back(token);
    }
}

std::string FF::nextToken()
{
    std::string buf;
    char c;

    while (m_fhPtr->nextc(c) != EOF)
    {
        if (c == ' ' || c == '|')
        {
            if (buf.length() == 0)
            {
                continue;
            }

            return buf;
        }
        else if (c == '\n' || c == '\t')
        {
            if (buf.length() != 0)
            {
                m_fhPtr->ungetc();
            }
            else
            {
                buf.push_back('$');
            }
            return buf;
        }
        else
        {
            buf.push_back(c);
        }
    }

    if (buf.length() != 0)
    {
        return buf;
    }
    else
    {
        buf = "$$";
        return buf;
    }
}

void FF::first(std::string &nt, std::vector<std::string> &vs)
{
    std::string var_str;
    for (auto itv = m_defs.begin(); itv != m_defs.end(); itv++)
    {
        if (nt == (*itv)[0])
        {
            var_str = (*itv)[1];
            if (isTerminal(var_str))
            {
                var_str.push_back(' ');
                vs.push_back(var_str);
            }
            else {
                first(var_str, vs);
            }
        }
    }
}

void FF::follow(std::string &nt, std::vector<std::string> &vs)
{
    for (auto itv = m_defs.begin(); itv != m_defs.end(); itv++)
    {
        // if it's a start symbol.
        if (itv == m_defs.begin() && nt == (*itv)[0])
        {
            vs.push_back("$");
            std::cout << (*itv)[0] << " ";
        }

        // now loop current definition.
        // skip the first nonterminal
        for (auto its = (*itv).begin()+1; its != (*itv).end(); its++)
        {
            // find nt in the definition
            // then find what it followed by.
            if ((*its) == nt)
            {
                // error
                if ((its + 1) != (*itv).end() - 1)
                {
                    followFirst(*(its + 1), vs);
                }
                // if nt is the last element
                // then follow(nt) is equal follow(the start nonterminal in the definition)
                if ((its + 1) == ((*itv).end() - 1) && \
                    nt != (*itv).front())
                {
                    follow((*itv).front(), vs);
                }
            }
        }
    }
}

void FF::followFirst(std::string &t, std::vector<std::string> &vs)
{
    if (isTerminal(t))
    {
        vs.push_back(t);
    }

    for (auto itv = m_defs.begin(); itv != m_defs.end(); itv++)
    {
        if (itv == m_defs.begin() && t == (*itv)[0])
        {
            if (isTerminal((*itv)[1]))
            {
                vs.push_back((*itv)[1]);
            }
            else
            {
                followFirst((*itv)[1], vs);
            }
        }
    }
}

bool FF::isTerminal(std::string &arg_str) const
{
    if (std::find(m_nts.begin(), m_nts.end(), arg_str) != m_nts.end())
    {
        return false;
    }
    else
    {
        return true;
    }
}

void FF::config(int index, const char *value)
{
    m_config[index] = value;
}

const char* FF::config(int index) const
{
    return m_config[index];
}

bool FF::error() const
{
    return m_err;
}

const char* FF::getMessage() const
{
    return m_message.c_str();
}