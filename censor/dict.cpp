#include "dict.h"
#include "MyEx.h"

Dict::Dict(const string& fname)
try
    : file(fname)
{
    stream = file.ReadFile();
}
catch(const exception& e)
{
    throw MyEx("Dict::Dict() "  + fname + ".\n\t" + string(e.what()));
}
