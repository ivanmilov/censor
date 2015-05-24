#include "dictrestrict.h"

template<class T>
DictRestrict<T>::DictRestrict(const string &fname, Encoding enc)
try
    : Dict(fname)
{
    makeDict(enc);
}
catch(const exception& e)
{
    throw MyEx("DictRestrict::DictRestrict() "  + fname + ".\n\t" + string(e.what()));
}

template<class T>
bool DictRestrict<T>::check(const T& word) const
{
    string::size_type len = word.length();
    auto sizegroup = dict.find(len);
    if(sizegroup != dict.end())
    {
        auto sizeset = sizegroup->second;
        return (sizeset.find(word) != sizeset.end());
    }
    return false;
}

// I didnt want reinvent the wheel,
// so I just used properties of map and set
// to achieve fast enough for this task searching
template<class T>
void DictRestrict<T>::makeDict(Encoding enc)
{
    // read stream line by line
    for (string line; getline(*stream, line); )
    {
        string::size_type len = line.length();
        // avoid empty lines
        if(len > 0)
        {
            // find/create set with specified length of word
            // and insert word to it
            T val = Encoder::AsciiToUTF<T>(line, enc);
            typename T::size_type key = val.length();
            dict[key].insert(val);
        }
    }
}


