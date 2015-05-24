#ifndef CENSOR_H
#define CENSOR_H

#include "includes.h"
#include "encoder.h"
#include "dictreplace.h"
#include "dictrestrict.h"

const int DEFBUFSIZE = 1024;

template<class T>
class Censor
{
public:
    Censor(const string& source, const string& dest, const string& replace, const string& restr, Encoding enc, bool rep, int bufsz = DEFBUFSIZE);
    Censor(const Censor&) = delete;
    Censor() = delete;

private:
    // initialize dictionaries according to file encoding
    void prepareDicts(Encoding enc);

    // read file block by block and check them
    void startChecking();

    // gets next block from file ended by any space symbol
    string readBytes();

    // checks for a match every word from block
    void checkBlock(const string &block);

    // writes BOM to result file if present
    void writeBom();

    // prints position and bytes for replasement
    void printreport(const T& from, const T& to, int pos) const;

    string fileNameSrc;
    string fileNameDest;
    string fileNameRepl;
    string fileNameRestr;

    unique_ptr<DictReplace<T>> dictRepl;
    unique_ptr<DictRestrict<T>> dictRestr;
    unique_ptr<CFile> finput;
    unique_ptr<CFile> foutput;

    Encoding encoding;
    bool report;
    int offset;         // current input file offset
    int BUFSZ;          // reading buffer size
};

#include "censor.cpp"
#endif // CENSOR_H
