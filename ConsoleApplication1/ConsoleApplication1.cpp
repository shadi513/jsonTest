
#pragma comment(lib, "urlmon.lib")
#include <urlmon.h>
#include <iostream>
#include <stdio.h>
#include "rapidjson/document.h"
#include <vector>
#include <assert.h>
#include <map>
#include <time.h>
#include "strptime.h"



struct actor
{
    std::string name;
    int age;
    std::string Born_At;
    std::string Birthdate;
}ac;

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
unsigned long bytesRead;
tm datetime;

vector<actor> v;

int main(int argc, TCHAR* argv[])
{
    printf("Retrieving JSON remote file from %s\r\n",URL.c_str());
    if (URLOpenBlockingStreamA(0, URL.c_str(), &stream, 0, 0) != 0)
    {
        cout << "failed to get file" << endl;
        return -1;
    }
    
    hFile = CreateFile(FN.c_str(), GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        while (true)
        {
            stream->Read(buff, sizeof(buff), &bytesRead);
            if(!bytesRead) break;
            json.append(buff, bytesRead);
            bErrorFlag = WriteFile(hFile, buff, bytesRead, &dwBytesWritten, NULL);
            if (bErrorFlag) wprintf(L"File %s saved successfully.\r\n", FN.c_str()); else printf("Error saving file\r\n");
        }

        //Print JSON string
        //cout << json << endl;

        //Analyze JSON string using tencent/rapidjson library
        Document d;
        d.Parse(json.c_str());
        assert(d["Actors"].IsArray());
        Value& a = d["Actors"];
        for (auto& val : a.GetArray())
        {
            //fill actor object (ac) with the data from JSON file
            ac.name = val["name"].GetString();
            ac.age = val["age"].GetInt();
            ac.Born_At = val["Born At"].GetString();
            ac.Birthdate = val["Birthdate"].GetString();

            //Put the data into vector container (v) with structure match the json format (ac)
            v.push_back(ac);

            //Analyze Date string to date and time parts
            strptime(ac.Birthdate.c_str(), "%d/%m/%Y", &datetime);
            //Hours and minutes and seconds are equals 0

            //Print the name and the Age into console
            cout << "Name: " << ac.name << " Age: " << (int)ac.age << endl;
        }

    }
    else
    {
        cout << "Cannot write file\r\n" << endl;
        return -1;
    }
    stream->Release();
    CloseHandle(hFile);
    printf("Vector size: %i\r\n", v.size());
    printf("Done.\r\n");
    return 0;
}
