#ifndef FF_H
#define FF_H

#include "file-helper.h"
#include <iostream>
#include <vector>

typedef enum
{
    CONFIG_ASSIGN,
} CONFIG_INDEX;

class FF
{
public:
    FF();
    ~FF();

    void input(const char *);
    void parse();
    void output(const char *);
    void config(int, const char*);
    const char* config(int) const;
    std::string nextToken();
    bool isTerminal(std::string &) const;

    void first(std::string &, std::vector<std::string> &);
    void follow(std::string &nt, std::vector<std::string> &vs);

    bool error() const;
    const char* getMessage() const;

private:
    void followFirst(std::string &t, std::vector<std::string> &vs);

private:
    std::vector<std::string> m_nts;
    std::vector<std::vector<std::string> > m_defs;
    FileHelper *m_fhPtr;
    int m_err;
    std::string m_message;
    char const *m_config[1];
};

#endif /* FF_H */