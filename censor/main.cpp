#include <iostream>
#include "censor.h"

using namespace std;

int usage()
{
    cout << "Usage:\n"
         << "$ ./Censor input_file "
         << " output_file"
         << " replacements_file"
         << " restrictions_file"
         << " [report]\n\n"
         << "report - is any char, \n"
         << "if [report] presence - report while replace\nelse - silence replace\n"
         << "Program will print \"Done\" at the end"
         << endl;
    return 0;
}

const int MAXMEM = 1024 * 1024 * 100;

int main(int argc, char* argv[])
{
    try
    {
        if(argc != 5 && argc != 6)
            return usage();

        string src{argv[1]};
        string out{argv[2]};
        string rpls{argv[3]};
        string rstr{argv[4]};

        bool report = argc == 6;

        // getting file encoding
        Encoding enc = Encoder::getFileEncoding(src);
        switch (enc)
        {
            case u16be:
            case u16le:
                Censor<u16string>(src,out,rpls,rstr, enc, report, MAXMEM);
                break;
            case u32le:
            case u32be:
                Censor<u32string>(src,out,rpls,rstr, enc, report, MAXMEM);
                break;
            case u8:
            case nobom:
                Censor<string>(src,out,rpls,rstr, enc, report, MAXMEM);
            break;
        default:
            break;
        }
        cout << "Done" << endl;
    }
    catch(MyEx& e)
    {
        if(!e.isOk())
        {
            cout << e.what() << endl;

        }
    }
    catch(exception& e)
    {
        cout << e.what()<< endl;
    }

//    getchar();
    return 0;
}

