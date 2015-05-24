#include "dictreplace.h"

template <class T>
DictReplace<T>::DictReplace(const string &fname, Encoding enc)
try
    : Dict(fname)
{
    makeDict(enc);
}
catch(const exception& e)
{
    throw MyEx("DictReplace::DictReplace() "  + fname + ".\n\t" + string(e.what()));
}

template <class T>
bool DictReplace<T>::check(const T &word, T &replacement) const
{
    typename T::size_type len = word.length();

    auto sizegroup = dict.find(len);
    if(sizegroup != dict.end())
    {
        auto sizemap = sizegroup->second;   // map with words same length
        auto res = sizemap.find(word);
        if (res != sizemap.end())
        {
            replacement = res->second;
            return true;
        }
    }
    return false;
}

template <class T>
void DictReplace<T>::removeDublicates(unique_ptr<DictRestrict<T>> &another)
{
    // checks every word from this dictionary with another
    for(auto it = dict.begin(); it != dict.end(); )
    {
        for(auto jt = it->second.begin(); jt != it->second.end(); )
        {
            // if find matching - delete element
            if(another->check(jt->first))
                jt = it->second.erase(jt);
            else
                ++jt;
        }
        if(it->second.size() == 0)
            it = dict.erase(it);
        else
            ++it;
    }
}

// I didnt want reinvent the wheel,
// so I just used properties of map and set
// to achieve fast enough for this task searching
template <class T>
void DictReplace<T>::makeDict(Encoding enc)
{
    string key, val;
    string::size_type pos;
    string delimiter{" | "};
    string::size_type dlen = delimiter.length();

    // read stream line by line
    for (string line; getline(*stream, line); )
    {
        pos = line.find(delimiter);
        if(string::npos != pos)
        {
            key = string(line, 0, pos);
            val = string(line, pos + dlen, line.length() - pos - dlen);

            T valt = Encoder::AsciiToUTF<T>(val, enc);
            T keyt = Encoder::AsciiToUTF<T>(key, enc);
            typename T::size_type keytlen = keyt.length();
            dict[keytlen][keyt] = valt;
        }
    }
}

