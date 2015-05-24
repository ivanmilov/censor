#ifndef DICTRESTRICT_H
#define DICTRESTRICT_H

#include "includes.h"
#include "encoder.h"
#include "dict.h"

template<class T>
class DictRestrict : public Dict
{
public:
    DictRestrict(const string& fname, Encoding enc);

    DictRestrict() = delete;
    DictRestrict(const Dict&) = delete;

    // looks for coincidence at dictionary
    // returns true if coincidence appear
    bool check(const T &word) const;

private:
    void makeDict(Encoding enc);

    map<typename T::value_type, set<T>> dict;
};
#include "dictrestrict.cpp"

#endif // DICTRESTRICT_H
