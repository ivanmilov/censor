#ifndef DICT_H
#define DICT_H

#include "includes.h"
#include "cfile.h"

using namespace std;
using pss = unique_ptr<stringstream>;

class Dict
{

public:
    Dict(const string &fname) ;
    Dict() = delete;
    Dict(const Dict&) = delete;
    virtual ~Dict() = default;

protected:
    pss stream;
    CFile file;
};

#endif // DICT_H
