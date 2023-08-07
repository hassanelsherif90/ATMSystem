#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdlib.h>
using namespace std;

const string FileClient = "Clients.txt";

struct stClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance = 0;
    bool MarkForDelet = false;
};

vector<string> SplitString(string S, string Delim)
{
    size_t pos = 0;
    string sWord;
    vector<string> vString;

    while ((pos = S.find(Delim)) != std::string::npos)
    {
        sWord = S.substr(0, pos);
        if (sWord != "")
        {
            vString.push_back(sWord);
        }
        S.erase(0, pos + Delim.length());
    }
    if (S != "")
    {
        vString.push_back(S);
    }
    return vString;
}

stClient ConvertLineToRecord(string stLine, string Parameter = "/##/")
{
    stClient Clients;
    vector<string> vClient = SplitString(stLine, Parameter);
    Clients.AccountNumber = vClient[0];
    Clients.PinCode = vClient[1];
    Clients.Name = vClient[2];
    Clients.Phone = vClient[3];
    Clients.AccountBalance = stod(vClient[4]);
    return Clients;
}

vector<stClient> LoadFileClientDate(string FileName)
{
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    vector<stClient> vClient;
    if (MyFile.is_open())
    {
        string Line;
        stClient Client;
        while (getline(MyFile, Line))
        {
            Client = ConvertLineToRecord(Line);
            vClient.push_back(Client);
        }
        MyFile.close();
    }
    return vClient;
}
stClient CurrentClient;

bool FindUserByAccountNumberAndPinCode(string AccountNumber, string PinCode, stClient &Client)
{
    vector<stClient> vClient = LoadFileClientDate(FileClient);
    for (stClient C : vClient)
    {
        if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

bool LoadAccountInfo(string AccountNumber, string PinCode)
{
    if (FindUserByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient))
        return true;
    else
        return false;
}

void Login()
{
    bool LoginFaild = false;
    string AccountNumber, PinCode;

    do
    {
        system("cls");
        cout << "\n==============================================================\n";
        cout << "\t\t\tLogin Screen";
        cout << "\n==============================================================\n";

        if (LoginFaild)
        {
            cout << "\nInvaliad AccountNumber / PinCode ? ";
        }

        cout << "Enter Account Number ? ";
        getline(cin >> ws, AccountNumber);

        cout << "Enter Pin Code ? ";
        cin >> PinCode;

        LoginFaild = !LoadAccountInfo(AccountNumber, PinCode);
    } while (LoginFaild);

    cout << "Done";
}

int main()
{
    Login();
    system("pause>0");
}
