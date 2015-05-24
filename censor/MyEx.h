#ifndef MYEX_H
#define MYEX_H

#include "includes.h"

class MyEx : public exception
{
public:
    MyEx(const string& what, bool ok = false):reason(what), ok(ok){}
    const char* what() const noexcept
    {
        return reason.c_str();
    }

    // not exception, just exit from application
    bool isOk() const {return ok;}

private:
    string reason;
    bool ok;
};

#endif // MYEX_H
