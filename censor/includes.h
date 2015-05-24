#ifndef INCLUDS
#define INCLUDS

#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>

#include <iostream>

using namespace std;

enum Encoding
{
    u16be = 0xa,  //1010
    u16le = 0x2,  //0010
    u8    = 0x3,  //0011
    u32be = 0xc,  //1100
    u32le = 0x4,  //0100
    nobom = 0x1   //0001
};
const int fBigEndian = 0x8;
const int fBOMSize = 0x7;
const int fIndepBOM = 0x1;    // char size independent from BOM (utf8 and ascii)

#endif // INCLUDS
