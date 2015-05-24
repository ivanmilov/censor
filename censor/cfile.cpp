#include "cfile.h"
#include "MyEx.h"

CFile::CFile(const string &fname, bool writeable)
try
    : filename(fname), fileLenght(-1)
{
    // set exception mask
    fs.exceptions(fstream::failbit);
    auto mask = writeable ? ios_base::out : ios_base::in;
    fs.open(fname, mask);
}
catch(MyEx& e)
{
    if(e.isOk()) throw e;
    else throw MyEx("CFile::CFile() Can't open "  + filename + ".\n\t" + string(e.what()));
}
catch(exception& e)
{
    throw MyEx("CFile::CFile() Can't open "  + filename + ".\n\t" + string(e.what()));
}

CFile::~CFile() throw ()
{
    if(fs.is_open())
    {
        try
        {
            fs.exceptions(fstream::failbit);
            fs.close();
        }
        catch(const exception& e )
        {
            throw MyEx("CFile::~CFile() Can't close "  + filename + ".\n" + string(e.what()));
        }
    }
}

int CFile::getFileLen() throw ()
{
    if(fileLenght >=0 )
        return fileLenght;

    try
    {
        fs.exceptions(fstream::badbit);
        fs.seekg(0, fs.end);
        fileLenght  = fs.tellg();
        fs.seekg(0, fs.beg);
        return fileLenght;
    }
    catch(const ios_base::failure& e )
    {
        throw MyEx("CFile::getFileLen() Can't count filelength "  + filename + ".\n\t" + string(e.what()));
    }
}

pss CFile::ReadFile(const int size) throw ()
{
    try
    {
        int length = size;
        // if size is not set - read whole file
        if(0 == length)
            length = getFileLen();

        // reserve buffer
        string buf;
        buf.resize(length, '\n');

        // read file to buffer
        fs.read(&*buf.begin(), length);

        fs.close();
        // make string stream from buffer
        return pss (new stringstream{buf});
    }
    catch(const MyEx& e)
    {
        throw e;
    }
    catch (const exception& e)
    {
        throw MyEx("Read file "  + filename + " exception.\n" + string(e.what()));
    }
}

int CFile::ReadBuf(const int size, const int off, string &buf) throw()
{
    try
    {
        int length = size;
        if(getFileLen() < length + off)
            length = getFileLen() - off;

        if ( length <= 0)
            return -1;

        // reserve buffer
        buf.resize(length,1);

        // go to offset
        fs.seekg(off, fs.beg);
        // read file to buffer
        fs.read(&*buf.begin(), length);

        return length;
    }
    catch(const MyEx& e)
    {
        throw e;
    }
    catch (const exception& e)
    {
        throw MyEx("Read file "  + filename + " exception.\n" + string(e.what()));
    }
}

void CFile::WriteBuf(const string &buf)
{
    try
    {
        fs.write(buf.c_str(), buf.length());
    }
    catch (const exception& e)
    {
        throw MyEx("Write file "  + filename + " exception.\n" + string(e.what()));
    }
}
