#include <cstring>   /* memcmp() */
#include "encoder.h"
#include "MyEx.h"

const char space[] = {0x20, 0x9, 0xa, 0xb, 0xc, 0xd}; // ' ' \t \n \v \f \r

Encoding Encoder::getFileEncoding(const string &filename)
{
    try
    {
        fstream fs;
        fs.exceptions(fstream::failbit | fstream::badbit);
        fs.open(filename, ios_base::in | ios_base::binary);

        fs.seekg(0, fs.end);
        int fileLenght  = fs.tellg();
        fs.seekg(0, fs.beg);

        if(fileLenght < 2)
        {
            fs.close();
            return nobom;
        }

        const int BOMLEN = min(fileLenght, 4);
        // reserve buffer
        ByteArray bom( new Byte[BOMLEN], default_delete<Byte[]>());

        // read file to buffer
        fs.read((char*)bom.get(), BOMLEN);
        fs.close();

        static const char *UTF_16_BE_BOM = "\xFE\xFF";
        static const char *UTF_16_LE_BOM = "\xFF\xFE";
        static const char *UTF_8_BOM = "\xEF\xBB\xBF";
        static const char *UTF_32_BE_BOM = "\x00\x00\xFE\xFF";
        static const char *UTF_32_LE_BOM = "\xFF\xFE\x00\x00";

        if (memcmp(bom.get(), UTF_8_BOM, 3) == 0)
            return u8;

        else if (memcmp(bom.get(), UTF_32_LE_BOM, 4) == 0)
            return u32le;

        else if (memcmp(bom.get(), UTF_32_BE_BOM, 4) == 0)
            return u32be;

        else if (memcmp(bom.get(), UTF_16_LE_BOM, 2) == 0)
            return u16le;

        else if (memcmp(bom.get(), UTF_16_BE_BOM, 2) == 0)
            return u16be;

        return nobom;
    }
    catch(const exception& e )
    {
        throw MyEx("Encoder::getFileEncoding() Can't open "  + filename + ".\n" + string(e.what()));
    }
}

u16string Encoder::asciiToUTF16s(const string& src, bool BE /*= true*/)
{
    u16string res;
    res.reserve(src.length());

    for(const auto& c: src)
    {
        if(BE)
            res.push_back(c<<8);
        else
            res.push_back(c);
    }

    return res;
}

u32string Encoder::asciiToUTF32s(const string& src, bool BE /*= true*/)
{
    u32string res;
    res.reserve(src.length());

    for(const auto& c: src)
    {
        if(BE)
            res.push_back(c<<24);
        else
            res.push_back(c);
    }

    return res;
}

string::size_type Encoder::getLastSpacePos(const string &rawbuf, Encoding enc, const string::size_type defpos)
{
    auto pos = rawbuf.find_last_of(space, defpos);
    if(pos == string::npos)
        return 0;

    // if little endian -> add last nulls
    auto tmp = pos;
    if(tmp > 0 && (enc & fIndepBOM) == 0 && (enc & fBigEndian) == 0)
        tmp += (enc & fBOMSize) - 1 ;

    // not necessary because size of the block is divisible by current utf charsize
    if(tmp >= rawbuf.length())
        tmp = getLastSpacePos(rawbuf, enc, pos);

    return tmp;
}

string::size_type Encoder::getSpacePos(const string &buf, Encoding enc, const string::size_type defpos)
{
    auto pos = buf.find_first_of(space, defpos);
    if(pos == string::npos)
        return buf.length();

    // if big endian -> remove prev nulls
    auto tmp = pos;
    if(tmp > 0 && (enc & fBigEndian) == fBigEndian)
        tmp -= (enc & fBOMSize) - 1 ;

    return tmp;
}

string::size_type Encoder::getNonSpacePos(const string &buf, Encoding enc, const string::size_type defpos)
{
    auto buflen = buf.length();
    auto pos = buf.find_first_not_of(space, defpos);
    auto off = defpos;
    while( pos != string::npos &&
           pos != buflen &&
           buf.at(pos) == '\0')
        pos = buf.find_first_not_of(space, ++off);

    if(pos == string::npos || pos == buflen)
        return buflen;

    // if big endian -> remove prev nulls
    auto tmp = pos;
    if( tmp > 0 &&
        (enc & fBigEndian) == fBigEndian &&
        buf.at(tmp - (enc & fBOMSize) + 1) == '\0' )
        tmp -= (enc & fBOMSize) - 1 ;

    return tmp;
}
