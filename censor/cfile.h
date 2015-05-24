#ifndef CFILE_H
#define CFILE_H

#include "includes.h"

using namespace std;
using pss = unique_ptr<stringstream>;

class CFile
{
public:
    CFile() = delete;
    CFile(const CFile&) = delete;
    CFile(const string& fname, bool writeable = false) ;
    ~CFile() throw ();

    // returns file length in bytes
    int getFileLen() throw ();

    // reads whole file and returns stringstream( for small files)
    pss ReadFile(const int size = 0) throw ();

    // returns byte buffer with size "size" from offset "off"
    int ReadBuf(const int size, const int off, string& buf) throw();

    // writes buffer to output file
    void WriteBuf(const string& buf);

private:
    fstream fs;
    string filename;
    int fileLenght;
    Encoding fencoding;
};

#endif // CFILE_H
