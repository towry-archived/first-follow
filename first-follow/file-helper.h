#ifndef FILE_HELPER_H
#define FILE_HELPER_H

#include <iostream>
#include <fstream>

class FileHelper {
public:
    FileHelper();
    ~FileHelper();

    void in(const char*);
    void out(const char*);
    const char& nextc(char &) const;
    const char ungetc() const;
    bool error();
    std::string getError();

private:
    std::ifstream m_is;
    std::filebuf *m_fbPtr;
    std::string m_serror;
};

#endif // FILE_HELPER_H