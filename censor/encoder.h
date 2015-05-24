#ifndef ENCODER_H
#define ENCODER_H

#include "includes.h"
#include "MyEx.h"

class Encoder
{
private:

    using Byte = int8_t;
    using ByteArray = shared_ptr<Byte>;

    // asciiToUTFxx converts ascii string to utf string with BOM (BE - BigEndian / LE - LittleEndian)
    static u16string asciiToUTF16s(const string& src, bool BE = true);
    static u32string asciiToUTF32s(const string& src, bool BE = true);

public:
    // returs encoding of current file
    static Encoding getFileEncoding(const string& filename);

    //returns last space position of block
    static string::size_type getLastSpacePos(const string& rawbuf, Encoding enc, const string::size_type defpos = string::npos);

    //returns next space position
    static string::size_type getSpacePos(const string& buf, Encoding enc, const string::size_type pos = 0);

    // returns next nonspace position
    static string::size_type getNonSpacePos(const string& buf, Encoding enc, const string::size_type pos = 0);

    template<class T>
    static T AsciiToUTF( string& s, Encoding enc)
    {
        try
        {
            if(typeid(T) == typeid(u16string))
            {
                u16string es = asciiToUTF16s(s, (enc & fBigEndian) == fBigEndian);
                T* ts = reinterpret_cast<T*>(&es);
                return *ts;
            }
            if(typeid(T) == typeid(u32string))
            {
                u32string es = asciiToUTF32s(s, (enc & fBigEndian) == fBigEndian);
                T* ts = reinterpret_cast<T*>(&es);
                return *ts;
            }
            if(typeid(T) == typeid(string))
            {

                T* ts = reinterpret_cast<T*>(&s);
                return *ts;
            }
            return T();
        }
        catch(exception& e)
        {
            throw MyEx("Encoder::Encoder() string: "  + s + ".\n\t" + string(e.what()));
        }
    }
};

#endif // ENCODER_H
