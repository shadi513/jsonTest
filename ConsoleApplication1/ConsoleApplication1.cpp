
#pragma comment(lib, "urlmon.lib")
#include <urlmon.h>
#include <iostream>
#include <stdio.h>
#include "rapidjson/document.h"
#include <vector>
#include <assert.h>
#include <map>
#include <time.h>



struct actor
{
    string name;
    int age;
    string Born_At;
    string Birthdate;
};

using namespace std;
using namespace rapidjson;

char buff[1000];
HANDLE hFile;
DWORD dwBytesWritten = 0;
BOOL bErrorFlag = FALSE;
string URL = "http://mqplanet.com/test.json";
string json;
wstring FN = L"test.json";
vector<actor> actors;
IStream* stream;
tm datetime;

map<string, string> mapForAttributeThatMatchesName(const Value& attributes, const string& findMemberName, const string& findMemberValue, const vector<string>& keysToRetrieve) {

    map<string, string> result;
    for (Value::ConstValueIterator itr = attributes.Begin(); itr != attributes.End(); ++itr) {

        const Value::ConstMemberIterator currentAttribute = itr->FindMember(findMemberName.c_str());
        if (currentAttribute != itr->MemberEnd() && currentAttribute->value.IsString()) {

            if (currentAttribute->value == findMemberValue.c_str()) {

                for (auto& keyToRetrieve : keysToRetrieve) {

                    const Value::ConstMemberIterator currentAttributeToReturn = itr->FindMember(keyToRetrieve.c_str());
                    if (currentAttributeToReturn != itr->MemberEnd() && currentAttributeToReturn->value.IsString()) {

                        result[keyToRetrieve] = currentAttributeToReturn->value.GetString();
                    }
                }
                return result;
            }
        }
    }
    return result;
}

int main(int argc, TCHAR* argv[])
{

    datetime.tm_year = 2022;
    datetime.tm_mon = 7;
    datetime.tm_mday = 7;
    datetime.tm_hour = 0;
    datetime.tm_min = 0;
    datetime.tm_sec = 0;



    printf("Getting remote file %s.\r\n",URL.c_str());
    if (URLOpenBlockingStreamA(0, URL.c_str(), &stream, 0, 0) != 0)
    {
        cout << "failed to get file";
        return -1;
    }

    unsigned long bytesRead;
    hFile = CreateFile(FN.c_str(), GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        while (true)
        {
            stream->Read(buff, sizeof(buff), &bytesRead);
            if(!bytesRead) break;
            json.append(buff, bytesRead);
            bErrorFlag = WriteFile(hFile, buff, bytesRead, &dwBytesWritten, NULL);
            wprintf(L"Wrote %d bytes to %s successfully.\r\n", dwBytesWritten, FN.c_str());
        };
        cout << json << endl;

        Document d;
        d.Parse(json.c_str());

        assert(d["Actors"].IsArray());

        Value& a = d["Actors"];
        map<string, string> mapForResult = mapForAttributeThatMatchesName(attributes, "name", "mass", keysToRetrieve);
        for (auto& mapItem : mapForResult) {

            cout << mapItem.first << ":" << mapItem.second << "\n";
        }
    }
    else
    {
        printf("Cannot write file\r\n");
    }
    stream->Release();
    CloseHandle(hFile);

    

    printf("Done.\r\n");
    return 0;
}

//// ONLY FOR WINDOWS 
//// NEEDS WINDOWS SET UP 
//// COMPILE USING Visual Studio Express, 
//// Visual C++ Express or any edition 
//// any version 
//#pragma comment(lib, "urlmon.lib")
//
//#include <urlmon.h>
//#include <iostream>
//
//#define getURL URLOpenBlockingStreamA
//
//using namespace std;
//
//// c program to download a file from url 
//int main()
//{
//
//    // Windows IStream interface 
//    IStream* stream;
//
//    const char* URL = "http://mqplanet.com/test.json";
//
//    // make a call to the URL 
//    // a non-zero return means some error 
//    if (getURL(0, URL, &stream, 0, 0))
//    {
//
//        cout << "Error occured.";
//
//        cout << "Check internet";
//
//        cout << "Check URL. Is it correct?";
//
//        return -1;
//
//    }
//
//    // this char array will be cyclically 
//    // filled with bytes from URL 
//    char buff[1000];
//
//    // we shall keep appending the bytes 
//    // to this string 
//    string s;
//
//    unsigned long bytesRead;
//
//    while (true)
//    {
//
//        // Reads a specified number of bytes 
//        // from the stream object into char 
//        // array and stores the actual 
//        // bytes read to "bytesRead" 
//        stream->Read(buff, 100, &bytesRead);
//
//        if (!bytesRead) break;
//
//        // append and collect to the string 
//        s.append(buff, bytesRead);
//
//    };
//
//    // release the interface 
//    // good programming practice 
//    stream->Release();
//
//    // display 
//    //cout << s << endl;
//    printf("%s",&s);
//    return 0;
//
//}