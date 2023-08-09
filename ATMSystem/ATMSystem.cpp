#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

const string FileClient = "Clients.txt";
void ShowMainMenuAtmystem();
void ShowMainMenuQuickWithdraw();
void Login();
void GotoBack();
short ReadChoose();

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

string ConvertRecordToLine(stClient Client, string Parameter = "/##/")
{
    string StLine;

    StLine += Client.AccountNumber + Parameter;
    StLine += Client.PinCode + Parameter;
    StLine += Client.Name + Parameter;
    StLine += Client.Phone + Parameter;
    StLine += to_string(Client.AccountBalance);

    return StLine;
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

void SaveToFile(string FileName, vector<stClient> &vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);

    string Line;
    if (MyFile.is_open())
    {
        for (stClient C : vClients)
        {
            if (C.MarkForDelet == false)
            {
                Line = ConvertRecordToLine(C);
                MyFile << Line << endl;
            }
        }
        MyFile.close();
    }
}

enum enMainMenuAtm
{
    enQuikWithdraw = 1,
    enNormalWithdraw = 2,
    enDeposit = 3,
    enCheckBalance = 4,
    enLogout = 5
};

bool DepositBalanceToClientByAccountNumber(string AccountNumber, vector<stClient> &vClients, double Amount)
{
    char Answer = 'n';
    cout << "\n\nAre you sure you want perfrom this transaction? y / n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        for (stClient &C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveToFile(FileClient, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;
                return true;
            }
        }
    }
    return false;
}

short getQuickWithDrawAmount(short MainMenueWithdraw)
{
    switch (MainMenueWithdraw)
    {
    case 1:
        return 20;
        break;
    case 2:
        return 50;
        break;
    case 3:
        return 100;
        break;
    case 4:
        return 200;
        break;
    case 5:
        return 400;
        break;
    case 6:
        return 600;
        break;
    case 7:
        return 800;
        break;
    case 8:
        return 1000;
        break;
    }
}

short ReadQuickWithDraw()
{
    short Choice;
    while (Choice < 1 || Choice > 9)
    {
        cout << "\nChooce what to withdraw from[1] to [8] ? ";
        cin >> Choice;
    }
    return Choice;
}

void PerForMainMenueQuickWithdraw(short QuickMenueWithdraw)
{
    if (QuickMenueWithdraw == 9)
    {
        return;
    }

    short WithDrawBalance = getQuickWithDrawAmount(QuickMenueWithdraw);
    while (WithDrawBalance > CurrentClient.AccountBalance)
    {
        cout << "Exceeds the balance , you can withdraw up to : " << endl;
        cout << "Press Anykey to Continue..........";
        system("pause>0");
        ShowMainMenuQuickWithdraw();
        return;
    }

    vector<stClient> vClient = LoadFileClientDate(FileClient);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, vClient, WithDrawBalance * -1);
    CurrentClient.AccountBalance -= WithDrawBalance;
}

int ReadWithDraw()
{
    int Amount;

    cout << "\nEnter Amount Multiple 5 s ?";
    cin >> Amount;

    while (Amount % 5 != 0)
    {
        cout << "\nEnter Amount Multiple 5's ?";
        cin >> Amount;
    }
    return Amount;
}

void PerForMainMenueQuickWithdraw()
{
    int WithDrawBalance = ReadWithDraw();

    while (WithDrawBalance > CurrentClient.AccountBalance)
    {
        cout << "Exceeds the balance , you can withdraw up to : " << endl;
        cout << "Press Anykey to Continue..........";
        system("pause>0");
        ShowMainMenuQuickWithdraw();
        return;
    }

    vector<stClient> vClient = LoadFileClientDate(FileClient);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, vClient, WithDrawBalance * -1);
    CurrentClient.AccountBalance -= WithDrawBalance;
}

void ShowMainMenuQuickWithdraw()
{
    system("cls");
    cout << "==================================================\n";
    cout << "\t\t\tQuick Withdraw\n";
    cout << "==================================================\n";
    cout << "\t[1] 20\t\t[2] 50.\n";
    cout << "\t[3] 100\t\t[4] 200.\n";
    cout << "\t[5] 400\t\t[6] 600.\n";
    cout << "\t[7] 800\t\t[8] 1000.\n";
    cout << "\t[9] Exit\n";
    cout << "==================================================\n";
    cout << "Your Balance is " << CurrentClient.AccountBalance << endl;
    PerForMainMenueQuickWithdraw(ReadQuickWithDraw());
}

void GotoBack()
{
    cout << "\nEnter anykey to back to Menue ............";
    system("pause>0");
    ShowMainMenuAtmystem();
}

void ShowMainMenuNormalWithdraw()
{
    system("cls");
    cout << "\n====================================================\n";
    cout << "\t\tScreen Normal Withdraw";
    cout << "\n====================================================\n";
    PerForMainMenueQuickWithdraw();
}

void ShowMainCheckBalance()
{
    system("cls");
    cout << "\n====================================================\n";
    cout << "\t\tCheck Balance Screen ";
    cout << "\n====================================================\n";
    cout << "you are Balance is : " << CurrentClient.AccountBalance << endl;
}

double ReadDepositAmount()
{
    double Amount;
    cout << "\nEnter Deposit Postive Amount ? ";
    cin >> Amount;
    while (Amount <= 0)
    {
        cout << "\nPlease Enter Deposit Postive Amount ? ";
        cin >> Amount;
    }
    return Amount;
}

void PerForMainMenueDeposit()
{
    double DepositAmount = ReadDepositAmount();

    vector<stClient> vClient = LoadFileClientDate(FileClient);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, vClient, DepositAmount);
    CurrentClient.AccountBalance += DepositAmount;
}

void ShowMainMenuDepositAmount()
{
    system("cls");
    cout << "\n====================================================\n";
    cout << "\t\tDeposit Screen";
    cout << "\n====================================================\n";
    PerForMainMenueDeposit();
}

void PerForMainMenueOptions(enMainMenuAtm MainMenueOptions)
{
    switch (MainMenueOptions)
    {
    case enMainMenuAtm::enQuikWithdraw:
        ShowMainMenuQuickWithdraw();
        GotoBack();
        break;
    case enMainMenuAtm::enNormalWithdraw:
        ShowMainMenuNormalWithdraw();
        GotoBack();
        break;
    case enMainMenuAtm::enDeposit:
        ShowMainMenuDepositAmount();
        GotoBack();
        break;
    case enMainMenuAtm::enCheckBalance:
        ShowMainCheckBalance();
        GotoBack();
        break;
    case enMainMenuAtm::enLogout:
        Login();
        break;
    }
}

short ReadChoose()
{
    short Number;
    cin >> Number;
    return Number;
}

void ShowMainMenuAtmystem()
{
    system("cls");
    cout << "==================================================\n";
    cout << "\t\t\tATM Main Menue Screen\n";
    cout << "==================================================\n";
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] Logout.\n";
    cout << "==================================================\n";
    cout << "Choose what do you want to do ? [1 to 5]? ";
    PerForMainMenueOptions((enMainMenuAtm)ReadChoose());
}

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

    ShowMainMenuAtmystem();
}

int main()
{
    Login();
    system("pause>0");
}
