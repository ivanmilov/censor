#ifndef DICTREPLACE_H
#define DICTREPLACE_H

#include "includes.h"
#include "dict.h"
#include "dictrestrict.h"
#include "encoder.h"

template <class T>
class DictReplace : public Dict
{
public:
    DictReplace(const string& fname, Encoding enc);

    DictReplace() = delete;
    DictReplace(const Dict&) = delete;

    // looks for coincidence at dictionary
    // returns true and replacement if coincidence appear
    bool check(const T& word, T& replacement) const;

    // remove dublicates to reduce number of elements for checking
    void removeDublicates(unique_ptr<DictRestrict<T> > &another);

private:
    void makeDict(Encoding enc);

    map<unsigned int, map<T, T>> dict;
};

#include "dictreplace.cpp"

#endif // DICTREPLACE_H
