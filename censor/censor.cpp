#include "censor.h"
#include "MyEx.h"

template<class T>
Censor<T>::Censor(const string &source, const string &dest, const string &replace, const string &restr, Encoding enc, bool rep, int bufsz)
try
    : fileNameSrc(source),
      fileNameDest(dest),
      fileNameRepl(replace),
      fileNameRestr(restr),
      encoding(enc),
      report(rep),
      offset(0),
      BUFSZ(bufsz)
{
    int characterSize = enc & fBOMSize;
    // utf8 char has variable length
    if(characterSize == 3)
        characterSize = 1;

    // adjust buffer size
    int rem = BUFSZ % characterSize;
    while(rem != 0)
        rem = (--BUFSZ) % characterSize;

    finput.reset(new CFile(source));
    if(finput->getFileLen() <= 0)
        return;

    foutput.reset(new CFile(dest, true));
    prepareDicts(enc);

    writeBom();
    startChecking();
}
catch(MyEx& e)
{
    if(e.isOk()) throw e;
    else throw MyEx("Censor::Censor.\n\t" + string(e.what()));
}
catch(exception& e)
{
    throw MyEx("Censor::Censor.\n\t" + string(e.what()));
}

template<class T>
void Censor<T>::prepareDicts(Encoding enc)
{
    try
    {
        dictRestr.reset(new DictRestrict<T>(fileNameRestr, enc));
        dictRepl.reset(new DictReplace<T>(fileNameRepl, enc));

        // remove dublicates from replasement dictionary
        dictRepl->removeDublicates(dictRestr);
    }
    catch(exception& e)
    {
        cout << "___prepareDicts catch"<< endl;
        throw MyEx("Can't prepare dicts.\n" + string(e.what()));
    }
}

template<class T>
void Censor<T>::startChecking()
{
    try
    {
        string buf;

        // read block
        buf = readBytes();
        while(buf.length() > 0)
        {
            // check words in block
            checkBlock(buf);

            buf = readBytes();
        }
    }
    catch(MyEx& e)
    {
        if(e.isOk()) throw e;
        else throw MyEx("Censor::startChecking.\n\t" + string(e.what()));
    }
    catch(exception& e)
    {
        throw MyEx("Censor::startChecking.\n\t" + string(e.what()));
    }
}

template<class T>
string Censor<T>::readBytes()
{
    string rawbuf, buf;
    // read block from current offset
    int readBytes = finput->ReadBuf(BUFSZ, offset, rawbuf);
    if(readBytes <= 0)
        return "";

    if(readBytes + offset < finput->getFileLen())
    {
        // extract block before last space symbol
        string::size_type pos = Encoder::getLastSpacePos(rawbuf, encoding);
        if(pos > 0)
            readBytes = pos + 1;
    }

    offset += readBytes;
    buf.resize(readBytes);
    rawbuf.copy(&*buf.begin(), readBytes);
    return buf;
}

template<class T>
void Censor<T>::checkBlock(const string& block)
{
    string::size_type posbeg{0}, posend{0}, spend{0}, lenword, lenwrite;
    T word, replacement;
    auto blocklen = block.length();
    auto charsz = sizeof(typename T::value_type);

    while(spend < blocklen - 1 || spend == 0)
    {
        // points on the first space character after word
        posend = Encoder::getSpacePos(block, encoding, posbeg);
        // points on the first char next word
        spend = Encoder::getNonSpacePos(block, encoding, posend);

        // length of checked word
        lenword = posend - posbeg;
        // length of bytes to write
        lenwrite = spend - posbeg;

        // reserve buf for word
        word.resize(lenword/charsz);
        // copy bytes to word
        block.copy((char*)&*word.begin(), lenword, posbeg);

        // check restiction
        if(dictRestr->check(word))
        {
            cout << "Restricted word at: " << offset - blocklen + posbeg << endl;
            throw MyEx("Restricted word founded!", true);
        }

        string tmp;
        // check replace
        if(dictRepl->check(word, replacement))
        {
            if(report)
                printreport(word, replacement, offset - blocklen + posbeg);

            // alloc bytes for replacement and spaces after
            auto lenrepl = replacement.length() * charsz;
            auto lensp = lenwrite - lenword;
            tmp.resize(lenrepl + lensp);
            // copy replacement
            replacement.copy((typename T::value_type*)&*tmp.begin(), replacement.length());
            // copy space symbols
            block.copy(&*(tmp.begin() + lenrepl), lensp, posbeg + lenword);
        }
        else
        {
            tmp.resize(lenwrite);
            block.copy(&*tmp.begin(), lenwrite, posbeg);
        }

        foutput->WriteBuf(tmp);

        posbeg = spend;
    }
}

template<class T>
void Censor<T>::writeBom()
{
    if(encoding == nobom)
        return;
    int bomsz = encoding & fBOMSize;
    string buf;
    offset += finput->ReadBuf(bomsz, offset, buf);
    foutput->WriteBuf(buf);
}

template<class T>
void Censor<T>::printreport(const T &from, const T &to, int pos) const
{
    cout << "replace word at: " << pos << "; ";
    for(auto c:from)
    {
        cout << c;
    }
    cout << " -> ";
    for(auto c:to)
    {
        cout << c;
    }
    cout<<hex<< endl;
}
