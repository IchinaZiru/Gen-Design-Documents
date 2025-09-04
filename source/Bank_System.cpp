#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;
const string ClientsFileName = "Clients.txt";

void ShowMainMenue();
void ShowTransactionsMenue();

struct sClient
{
  string AccountNumber;
  string PinCode;
  string Name;
  string Phone;
  double AccountBalance;
  bool MarkForDelete = false;
};

/** @brief 文字列を指定した区切り文字で分割する
	* @param S1 (string) 分割対象の文字列
	* @param Delim (string) 区切り文字
	* @return vector< string > 分割された文字列のリスト
	* @details 入力された文字列S1をDelimで指定された区切り文字で分割し、その結果を文字列のベクターとして返します。Delimが空文字列の場合、S1は一文字ずつ分割されます。
	*/
vector<string> SplitString(string S1, string Delim)
{
  vector<string> vString;

  short pos = 0;
  string sWord; // define a string variable

  // use find() function to get the position of the delimiters
  while ((pos = S1.find(Delim)) != std::string::npos)
  {
    sWord = S1.substr(0, pos); // store the word
    if (sWord != "")
    {
      vString.push_back(sWord);
    }

    S1.erase(0, pos + Delim.length()); /* erase() until positon and move to next word. */
  }

  if (S1 != "")
  {
    vString.push_back(S1); // it adds last word of the string.
  }

  return vString;
}

/** @brief 文字列をレコードに変換する関数
	* @param Line (string)  レコードに変換するための文字列
	* @param Seperator (string)  フィールドを区切るためのセパレータ（デフォルトは "#//#"）
	* @return sClient 変換されたレコード
	* @details この関数は、指定されたセパレータを使用して文字列をフィールドに分割し、その結果をsClient型のレコードに変換します。セパレータが指定されていない場合はデフォルトの "#//#" を使用します。
	*/
sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
  sClient Client;
  vector<string> vClientData;

  vClientData = SplitString(Line, Seperator);

  Client.AccountNumber = vClientData[0];
  Client.PinCode = vClientData[1];
  Client.Name = vClientData[2];
  Client.Phone = vClientData[3];
  Client.AccountBalance = stod(vClientData[4]); // cast string to double

  return Client;
}

/** @brief クライアントのレコードを一行の文字列に変換する
	* @param Client (sClient)  変換されるクライアントのレコード
	* @param Seperator (string)  レコードの各要素を分割するためのセパレータ。デフォルトは "#//#"
	* @return string 変換された一行の文字列
	* @details この関数は、クライアントのレコードを一行の文字列に変換します。各要素は指定されたセパレータで分割されます。セパレータが指定されない場合、デフォルトのセパレータ "#//#" が使用されます。
	*/
string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

  string stClientRecord = "";

  stClientRecord += Client.AccountNumber + Seperator;
  stClientRecord += Client.PinCode + Seperator;
  stClientRecord += Client.Name + Seperator;
  stClientRecord += Client.Phone + Seperator;
  stClientRecord += to_string(Client.AccountBalance);

  return stClientRecord;
}

/** @brief アカウント番号に基づいてクライアントが存在するかどうかを確認する関数
	* @param AccountNumber (string)  確認したいクライアントのアカウント番号
	* @param FileName (string)  クライアント情報が格納されているファイルの名前
	* @return bool クライアントが存在する場合はtrue、存在しない場合はfalseを返す
	* @details ファイルからクライアント情報を読み込み、指定されたアカウント番号のクライアントが存在するかどうかを確認します。
	*/
bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{

  vector<sClient> vClients;

  fstream MyFile;
  MyFile.open(FileName, ios::in); // read Mode

  if (MyFile.is_open())
  {

    string Line;
    sClient Client;

    while (getline(MyFile, Line))
    {

      Client = ConvertLinetoRecord(Line);
      if (Client.AccountNumber == AccountNumber)
      {
        MyFile.close();
        return true;
      }

      vClients.push_back(Client);
    }

    MyFile.close();
  }

  return false;
}

/** @brief 新規クライアントを読み込む
	* @return sClient 新規に読み込まれたクライアントの情報を保持するsClientオブジェクト
	* @details この関数は新規クライアントの情報を読み込み、その情報を保持するsClientオブジェクトを返します。
	*/
sClient ReadNewClient()
{
  sClient Client;

  cout << "Enter Account Number? ";

  // Usage of std::ws will extract allthe whitespace character
  getline(cin >> ws, Client.AccountNumber);

  while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
  {
    cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
    getline(cin >> ws, Client.AccountNumber);
  }

  cout << "Enter PinCode? ";
  getline(cin, Client.PinCode);

  cout << "Enter Name? ";
  getline(cin, Client.Name);

  cout << "Enter Phone? ";
  getline(cin, Client.Phone);

  cout << "Enter AccountBalance? ";
  cin >> Client.AccountBalance;

  return Client;
}

/** @brief ファイルからクライアントのデータを読み込み、それをsClient型のベクタとして返す関数
	* @param FileName (string)  クライアントのデータが保存されているファイルの名前
	* @return vector< sClient > ファイルから読み込んだクライアントのデータを格納したsClient型のベクタ
	* @details この関数は指定されたファイルからクライアントのデータを読み込みます。読み込んだデータはsClient型のベクタとして返されます。ファイルが存在しない場合や読み込みに失敗した場合のエラーハンドリングはこの関数の内部で行われます。
	*/
vector<sClient> LoadCleintsDataFromFile(string FileName)
{

  vector<sClient> vClients;

  fstream MyFile;
  MyFile.open(FileName, ios::in); // read Mode

  if (MyFile.is_open())
  {

    string Line;
    sClient Client;

    while (getline(MyFile, Line))
    {

      Client = ConvertLinetoRecord(Line);

      vClients.push_back(Client);
    }

    MyFile.close();
  }

  return vClients;
}

/** @brief クライアントのレコードを一行で出力する
	* @param Client (sClient) 出力するクライントのレコード情報
	* @details この関数は、引数で与えられたクライアントのレコード情報を一行で出力します。出力内容はクライアントの各フィールドがカンマ区切りの形式となります。出力は標準出力に送られます。*/
void PrintClientRecordLine(sClient Client)
{

  cout << "| " << setw(15) << left << Client.AccountNumber;
  cout << "| " << setw(10) << left << Client.PinCode;
  cout << "| " << setw(40) << left << Client.Name;
  cout << "| " << setw(12) << left << Client.Phone;
  cout << "| " << setw(12) << left << Client.AccountBalance;
}

/** @brief クライアントのレコードを一行で出力する
	* @param Client (sClient) 出力するクライントのレコード情報
	* @details この関数は、引数で与えられたクライアントのレコード情報を一行で出力します。出力内容はクライアントの各フィールドがカンマ区切りの形式となります。出力は標準出力に送られます。*/
void PrintClientRecordBalanceLine(sClient Client)
{

  cout << "| " << setw(15) << left << Client.AccountNumber;
  cout << "| " << setw(40) << left << Client.Name;
  cout << "| " << setw(12) << left << Client.AccountBalance;
}

/** @brief すべてのクライアントの画面を表示する
	* @details この関数は、現在接続されているすべてのクライアントの画面を表示します。各クライアントの画面は、それぞれ独立して表示されます。この関数を呼び出すと、すべてのクライアントの画面が一覧できます。具体的な表示方法やレイアウトは、実装に依存します。
	*/
void ShowAllClientsScreen()
{

  vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

  cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
  cout << "\n_______________________________________________________";
  cout << "_________________________________________\n"
       << endl;

  cout << "| " << left << setw(15) << "Accout Number";
  cout << "| " << left << setw(10) << "Pin Code";
  cout << "| " << left << setw(40) << "Client Name";
  cout << "| " << left << setw(12) << "Phone";
  cout << "| " << left << setw(12) << "Balance";
  cout << "\n_______________________________________________________";
  cout << "_________________________________________\n"
       << endl;

  if (vClients.size() == 0)
    cout << "\t\t\t\tNo Clients Available In the System!";
  else

    for (sClient Client : vClients)
    {

      PrintClientRecordLine(Client);
      cout << endl;
    }

  cout << "\n_______________________________________________________";
  cout << "_________________________________________\n"
       << endl;
}

/** @brief すべてのクライアントの画面を表示する
	* @details この関数は、現在接続されているすべてのクライアントの画面を表示します。各クライアントの画面は、それぞれ独立して表示されます。この関数を呼び出すと、すべてのクライアントの画面が一覧できます。具体的な表示方法やレイアウトは、実装に依存します。
	*/
void ShowTotalBalances()
{

  vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

  cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
  cout << "\n_______________________________________________________";
  cout << "_________________________________________\n"
       << endl;

  cout << "| " << left << setw(15) << "Accout Number";
  cout << "| " << left << setw(40) << "Client Name";
  cout << "| " << left << setw(12) << "Balance";
  cout << "\n_______________________________________________________";
  cout << "_________________________________________\n"
       << endl;

  double TotalBalances = 0;

  if (vClients.size() == 0)
    cout << "\t\t\t\tNo Clients Available In the System!";
  else

    for (sClient Client : vClients)
    {

      PrintClientRecordBalanceLine(Client);
      TotalBalances += Client.AccountBalance;

      cout << endl;
    }

  cout << "\n_______________________________________________________";
  cout << "_________________________________________\n"
       << endl;
  cout << "\t\t\t\t\t   Total Balances = " << TotalBalances;
}

/** @brief クライアントのレコードを一行で出力する
	* @param Client (sClient) 出力するクライントのレコード情報
	* @details この関数は、引数で与えられたクライアントのレコード情報を一行で出力します。出力内容はクライアントの各フィールドがカンマ区切りの形式となります。出力は標準出力に送られます。*/
void PrintClientCard(sClient Client)
{
  cout << "\nThe following are the client details:\n";
  cout << "-----------------------------------";
  cout << "\nAccout Number: " << Client.AccountNumber;
  cout << "\nPin Code     : " << Client.PinCode;
  cout << "\nName         : " << Client.Name;
  cout << "\nPhone        : " << Client.Phone;
  cout << "\nAccount Balance: " << Client.AccountBalance;
  cout << "\n-----------------------------------\n";
}

/** @brief アカウント番号を用いてクライアントを検索する関数
	* @param AccountNumber (string) 検索するクライアントのアカウント番号
	* @param vClients (vector< sClient >) クライアント情報が格納されたベクター
	* @param Client (sClient &) 検索結果を格納するためのクライアントオブジェクトの参照
	* @return bool 検索結果。クライアントが見つかった場合はtrue、見つからなかった場合はfalseを返す。
	* @details アカウント番号を用いてクライアント情報を検索します。見つかった場合は該当するクライアント情報をClientオブジェクトに格納し、trueを返します。見つからなかった場合はfalseを返します。
	*/
bool FindClientByAccountNumber(string AccountNumber, vector<sClient> vClients, sClient &Client)
{

  for (sClient C : vClients)
  {

    if (C.AccountNumber == AccountNumber)
    {
      Client = C;
      return true;
    }
  }
  return false;
}

/** @brief 与えられたアカウント番号に基づいてクライアントのレコードを変更する
	* @param AccountNumber (string)  変更を行いたいクライントのアカウント番号
	* @return sClient 変更後のクライアント情報を含むsClientオブジェクト
	* @details この関数は、指定されたアカウント番号を持つクライアントのレコードを変更します。変更後のクライアント情報はsClientオブジェクトとして返されます。
	*/
sClient ChangeClientRecord(string AccountNumber)
{
  sClient Client;

  Client.AccountNumber = AccountNumber;

  cout << "\n\nEnter PinCode? ";
  getline(cin >> ws, Client.PinCode);

  cout << "Enter Name? ";
  getline(cin, Client.Name);

  cout << "Enter Phone? ";
  getline(cin, Client.Phone);

  cout << "Enter AccountBalance? ";
  cin >> Client.AccountBalance;

  return Client;
}

/** @brief アカウント番号によってクライアントを削除マークします。
	* @param AccountNumber (string)  削除マークをつけるクライアントのアカウント番号
	* @param vClients (vector< sClient > &)  クライアント情報が格納されたベクター
	* @return bool 削除マークが正常につけられた場合はtrue、それ以外の場合はfalseを返します。
	* @details この関数は指定されたアカウント番号のクライアントを探し、見つかった場合にそのクライアントに削除マークをつけます。クライアントが見つからなかった場合や何らかのエラーが発生した場合はfalseを返します。
	*/
bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<sClient> &vClients)
{

  for (sClient &C : vClients)
  {

    if (C.AccountNumber == AccountNumber)
    {
      C.MarkForDelete = true;
      return true;
    }
  }

  return false;
}

/** @brief クライアントデータを指定のファイルに保存する関数
	* @param FileName (string)  クライアントデータを保存するためのファイル名
	* @param vClients (vector< sClient >)  保存するクライアントデータのベクター
	* @return vector< sClient > 保存に成功したクライアントデータのベクターを返す
	* @details この関数は、指定されたファイル名でクライアントのデータを保存します。保存に成功したクライアントのデータのみをベクターとして返します。保存に失敗した場合の処理はこの関数の中で行われます。
	*/
vector<sClient> SaveCleintsDataToFile(string FileName, vector<sClient> vClients)
{

  fstream MyFile;
  MyFile.open(FileName, ios::out); // overwrite

  string DataLine;

  if (MyFile.is_open())
  {

    for (sClient C : vClients)
    {

      if (C.MarkForDelete == false)
      {
        // we only write records that are not marked for delete.
        DataLine = ConvertRecordToLine(C);
        MyFile << DataLine << endl;
      }
    }

    MyFile.close();
  }

  return vClients;
}

/** @brief ファイルにデータ行を追加する
	* @param FileName (string) 追加先のファイル名
	* @param stDataLine (string) 追加するデータ行
	* @details この関数は指定されたファイル名のファイルに、指定されたデータ行を追加します。データ行は新たな行として追加されます。ファイルが存在しない場合は新規に作成されます。
void AddDataLineToFile(string FileName, string stDataLine)
{
  fstream MyFile;
  MyFile.open(FileName, ios::out | ios::app);

  if (MyFile.is_open())
  {

    MyFile << stDataLine << endl;

    MyFile.close();
  }
}

/** @brief すべてのクライアントの画面を表示する
	* @details この関数は、現在接続されているすべてのクライアントの画面を表示します。各クライアントの画面は、それぞれ独立して表示されます。この関数を呼び出すと、すべてのクライアントの画面が一覧できます。具体的な表示方法やレイアウトは、実装に依存します。
	*/
void AddNewClient()
{
  sClient Client;
  Client = ReadNewClient();
  AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

/** @brief すべてのクライアントの画面を表示する
	* @details この関数は、現在接続されているすべてのクライアントの画面を表示します。各クライアントの画面は、それぞれ独立して表示されます。この関数を呼び出すと、すべてのクライアントの画面が一覧できます。具体的な表示方法やレイアウトは、実装に依存します。
	*/
void AddNewClients()
{
  char AddMore = 'Y';
  do
  {
    // system("cls");
    cout << "Adding New Client:\n\n";

    AddNewClient();
    cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";

    cin >> AddMore;

  } while (toupper(AddMore) == 'Y');
}

/** @brief アカウント番号によってクライアントを削除マークします。
	* @param AccountNumber (string)  削除マークをつけるクライアントのアカウント番号
	* @param vClients (vector< sClient > &)  クライアント情報が格納されたベクター
	* @return bool 削除マークが正常につけられた場合はtrue、それ以外の場合はfalseを返します。
	* @details この関数は指定されたアカウント番号のクライアントを探し、見つかった場合にそのクライアントに削除マークをつけます。クライアントが見つからなかった場合や何らかのエラーが発生した場合はfalseを返します。
	*/
bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient> &vClients)
{

  sClient Client;
  char Answer = 'n';

  if (FindClientByAccountNumber(AccountNumber, vClients, Client))
  {

    PrintClientCard(Client);

    cout << "\n\nAre you sure you want delete this client? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
      MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
      SaveCleintsDataToFile(ClientsFileName, vClients);

      // Refresh Clients
      vClients = LoadCleintsDataFromFile(ClientsFileName);

      cout << "\n\nClient Deleted Successfully.";
      return true;
    }
  }
  else
  {
    cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
    return false;
  }
}

/** @brief アカウント番号によってクライアントを削除マークします。
	* @param AccountNumber (string)  削除マークをつけるクライアントのアカウント番号
	* @param vClients (vector< sClient > &)  クライアント情報が格納されたベクター
	* @return bool 削除マークが正常につけられた場合はtrue、それ以外の場合はfalseを返します。
	* @details この関数は指定されたアカウント番号のクライアントを探し、見つかった場合にそのクライアントに削除マークをつけます。クライアントが見つからなかった場合や何らかのエラーが発生した場合はfalseを返します。
	*/
bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient> &vClients)
{

  sClient Client;
  char Answer = 'n';

  if (FindClientByAccountNumber(AccountNumber, vClients, Client))
  {

    PrintClientCard(Client);
    cout << "\n\nAre you sure you want update this client? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {

      for (sClient &C : vClients)
      {
        if (C.AccountNumber == AccountNumber)
        {
          C = ChangeClientRecord(AccountNumber);
          break;
        }
      }

      SaveCleintsDataToFile(ClientsFileName, vClients);

      cout << "\n\nClient Updated Successfully.";
      return true;
    }
  }
  else
  {
    cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
    return false;
  }
}

/** @brief 指定された口座番号のクライアントに対して、指定された金額を預金します。
	* @param AccountNumber (string)  預金を行うクライアントの口座番号。
	* @param Amount (double)  預金する金額。
	* @param vClients (vector< sClient > &)  クライアント情報を保持するベクター。
	* @return bool 預金が成功した場合はtrue、それ以外の場合はfalseを返します。
	* @details この関数は、指定された口座番号のクライアントに対して指定された金額を預金します。預金を行うクライアントはvClientsベクターから検索されます。口座番号が存在しない場合や預金処理が何らかの理由で失敗した場合はfalseを返します。成功した場合はtrueを返します。 
	*/
bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector<sClient> &vClients)
{
  char Answer = 'n';

  cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
  cin >> Answer;
  if (Answer == 'y' || Answer == 'Y')
  {

    for (sClient &C : vClients)
    {
      if (C.AccountNumber == AccountNumber)
      {
        C.AccountBalance += Amount;
        SaveCleintsDataToFile(ClientsFileName, vClients);
        cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;

        return true;
      }
    }
    return false;
  }
}

/** @brief クライアントのアカウント番号を読み取る
	* @return string クライアントのアカウント番号を文字列として返す
	* @details この関数は、クライアントのアカウント番号を読み取り、それを文字列として返します。具体的な読み取り方法やアカウント番号の形式は、実装に依存します。
	*/
string ReadClientAccountNumber()
{
  string AccountNumber = "";

  cout << "\nPlease enter AccountNumber? ";
  cin >> AccountNumber;
  return AccountNumber;
}

/** @brief すべてのクライアントの画面を表示する
	* @details この関数は、現在接続されているすべてのクライアントの画面を表示します。各クライアントの画面は、それぞれ独立して表示されます。この関数を呼び出すと、すべてのクライアントの画面が一覧できます。具体的な表示方法やレイアウトは、実装に依存します。
	*/
void ShowDeleteClientScreen()
{
  cout << "\n-----------------------------------\n";
  cout << "\tDelete Client Screen";
  cout << "\n-----------------------------------\n";

  vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
  string AccountNumber = ReadClientAccountNumber();
  DeleteClientByAccountNumber(AccountNumber, vClients);
}

/** @brief すべてのクライアントの画面を表示する
	* @details この関数は、現在接続されているすべてのクライアントの画面を表示します。各クライアントの画面は、それぞれ独立して表示されます。この関数を呼び出すと、すべてのクライアントの画面が一覧できます。具体的な表示方法やレイアウトは、実装に依存します。
	*/
void ShowUpdateClientScreen()
{
  cout << "\n-----------------------------------\n";
  cout << "\tUpdate Client Info Screen";
  cout << "\n-----------------------------------\n";

  vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
  string AccountNumber = ReadClientAccountNumber();
  UpdateClientByAccountNumber(AccountNumber, vClients);
}

/** @brief すべてのクライアントの画面を表示する
	* @details この関数は、現在接続されているすべてのクライアントの画面を表示します。各クライアントの画面は、それぞれ独立して表示されます。この関数を呼び出すと、すべてのクライアントの画面が一覧できます。具体的な表示方法やレイアウトは、実装に依存します。
	*/
void ShowAddNewClientsScreen()
{
  cout << "\n-----------------------------------\n";
  cout << "\tAdd New Clients Screen";
  cout << "\n-----------------------------------\n";

  AddNewClients();
}

/** @brief すべてのクライアントの画面を表示する
	* @details この関数は、現在接続されているすべてのクライアントの画面を表示します。各クライアントの画面は、それぞれ独立して表示されます。この関数を呼び出すと、すべてのクライアントの画面が一覧できます。具体的な表示方法やレイアウトは、実装に依存します。
	*/
void ShowFindClientScreen()
{
  cout << "\n-----------------------------------\n";
  cout << "\tFind Client Screen";
  cout << "\n-----------------------------------\n";

  vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
  sClient Client;
  string AccountNumber = ReadClientAccountNumber();
  if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    PrintClientCard(Client);
  else
    cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";
}

/** @brief すべてのクライアントの画面を表示する
	* @details この関数は、現在接続されているすべてのクライアントの画面を表示します。各クライアントの画面は、それぞれ独立して表示されます。この関数を呼び出すと、すべてのクライアントの画面が一覧できます。具体的な表示方法やレイアウトは、実装に依存します。
	*/
void ShowEndScreen()
{
  cout << "\n-----------------------------------\n";
  cout << "\tProgram Ends :-)";
  cout << "\n-----------------------------------\n";
  cout << "  - Author: Oussama Azzouz";
}

/** @brief すべてのクライアントの画面を表示する
	* @details この関数は、現在接続されているすべてのクライアントの画面を表示します。各クライアントの画面は、それぞれ独立して表示されます。この関数を呼び出すと、すべてのクライアントの画面が一覧できます。具体的な表示方法やレイアウトは、実装に依存します。
	*/
void ShowDepositScreen()
{
  cout << "\n-----------------------------------\n";
  cout << "\tDeposit Screen";
  cout << "\n-----------------------------------\n";

  sClient Client;

  vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
  string AccountNumber = ReadClientAccountNumber();

  while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
  {
    cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
    AccountNumber = ReadClientAccountNumber();
  }

  PrintClientCard(Client);

  double Amount = 0;
  cout << "\nPlease enter deposit amount? ";
  cin >> Amount;

  DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);
}

/** @brief すべてのクライアントの画面を表示する
	* @details この関数は、現在接続されているすべてのクライアントの画面を表示します。各クライアントの画面は、それぞれ独立して表示されます。この関数を呼び出すと、すべてのクライアントの画面が一覧できます。具体的な表示方法やレイアウトは、実装に依存します。
	*/
void ShowWithDrawScreen()
{
  cout << "\n-----------------------------------\n";
  cout << "\tWithdraw Screen";
  cout << "\n-----------------------------------\n";

  sClient Client;

  vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
  string AccountNumber = ReadClientAccountNumber();

  while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
  {
    cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
    AccountNumber = ReadClientAccountNumber();
  }

  PrintClientCard(Client);

  double Amount = 0;
  cout << "\nPlease enter withdraw amount? ";
  cin >> Amount;

  // Validate that the amount does not exceeds the balance
  while (Amount > Client.AccountBalance)
  {
    cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
    cout << "Please enter another amount? ";
    cin >> Amount;
  }

  DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);
}

/** @brief すべてのクライアントの画面を表示する
	* @details この関数は、現在接続されているすべてのクライアントの画面を表示します。各クライアントの画面は、それぞれ独立して表示されます。この関数を呼び出すと、すべてのクライアントの画面が一覧できます。具体的な表示方法やレイアウトは、実装に依存します。
	*/
void ShowTotalBalancesScreen()
{

  ShowTotalBalances();
}

enum enTransactionsMenueOptions
{
  eDeposit = 1,
  eWithdraw = 2,
  eShowTotalBalance = 3,
  eShowMainMenue = 4
};

enum enMainMenueOptions
{
  eListClients = 1,
  eAddNewClient = 2,
  eDeleteClient = 3,
  eUpdateClient = 4,
  eFindClient = 5,
  eShowTransactionsMenue = 6,
  eExit = 7
};

/** @brief すべてのクライアントの画面を表示する
	* @details この関数は、現在接続されているすべてのクライアントの画面を表示します。各クライアントの画面は、それぞれ独立して表示されます。この関数を呼び出すと、すべてのクライアントの画面が一覧できます。具体的な表示方法やレイアウトは、実装に依存します。
	*/
void GoBackToMainMenue()
{
  cout << "\n\nPress any key to go back to Main Menue...";
  system("pause>0");
  ShowMainMenue();
}
/** @brief すべてのクライアントの画面を表示する
	* @details この関数は、現在接続されているすべてのクライアントの画面を表示します。各クライアントの画面は、それぞれ独立して表示されます。この関数を呼び出すと、すべてのクライアントの画面が一覧できます。具体的な表示方法やレイアウトは、実装に依存します。
	*/
void GoBackToTransactionsMenue()
{
  cout << "\n\nPress any key to go back to Transactions Menue...";
  system("pause>0");
  ShowTransactionsMenue();
}
/** @brief トランザクションメニューオプションを読み込む関数
	* @return short  選択されたメニューオプションを表す短い整数
	* @details この関数は、ユーザーによるトランザクションメニューの選択を読み込み、選択されたメニューオプションを表す短い整数を返します。
	*/
short ReadTransactionsMenueOption()
{
  cout << "Choose what do you want to do? [1 to 4]? ";
  short Choice = 0;
  cin >> Choice;

  return Choice;
}

/** @brief トランザクションメニューオプションを実行する関数
	* @param TransactionMenueOption (enTransactionsMenueOptions)  実行するトランザクションメニューオプションを指定します。
	* @details この関数は、指定されたトランザクションメニューオプションを実行します。具体的な動作は、指定されたオプションに依存します。
	*/
void PerfromTranactionsMenueOption(enTransactionsMenueOptions TransactionMenueOption)
{
  switch (TransactionMenueOption)
  {
  case enTransactionsMenueOptions::eDeposit:
  {
    system("cls");
    ShowDepositScreen();
    GoBackToTransactionsMenue();
    break;
  }

  case enTransactionsMenueOptions::eWithdraw:
  {
    system("cls");
    ShowWithDrawScreen();
    GoBackToTransactionsMenue();
    break;
  }

  case enTransactionsMenueOptions::eShowTotalBalance:
  {
    system("cls");
    ShowTotalBalancesScreen();
    GoBackToTransactionsMenue();
    break;
  }

  case enTransactionsMenueOptions::eShowMainMenue:
  {

    ShowMainMenue();
  }
  }
}

/** @brief すべてのクライアントの画面を表示する
	* @details この関数は、現在接続されているすべてのクライアントの画面を表示します。各クライアントの画面は、それぞれ独立して表示されます。この関数を呼び出すと、すべてのクライアントの画面が一覧できます。具体的な表示方法やレイアウトは、実装に依存します。
	*/
void ShowTransactionsMenue()
{
  system("cls");
  cout << "===========================================\n";
  cout << "\t\tTransactions Menue Screen\n";
  cout << "===========================================\n";
  cout << "\t[1] Deposit.\n";
  cout << "\t[2] Withdraw.\n";
  cout << "\t[3] Total Balances.\n";
  cout << "\t[4] Main Menue.\n";
  cout << "===========================================\n";
  PerfromTranactionsMenueOption((enTransactionsMenueOptions)ReadTransactionsMenueOption());
}

/** @brief トランザクションメニューオプションを読み込む関数
	* @return short  選択されたメニューオプションを表す短い整数
	* @details この関数は、ユーザーによるトランザクションメニューの選択を読み込み、選択されたメニューオプションを表す短い整数を返します。
	*/
short ReadMainMenueOption()
{
  short Choice = 0;
  do
  {
    cout << "Choose what do you want to do? [1 to 7]? ";
    cin >> Choice;
  } while (Choice < 1 || Choice > 7);

  return Choice;
}

/** @brief メインメニューオプションを実行する関数
	* @param MainMenueOption (enMainMenueOptions)  メインメニューのオプションを表す列挙型
	* @details この関数は、メインメニューのオプションを引数として受け取り、それに対応する操作を実行します。具体的な操作は、引数で指定されたオプションに依存します。
	*/
void PerfromMainMenueOption(enMainMenueOptions MainMenueOption)
{
  switch (MainMenueOption)
  {
  case enMainMenueOptions::eListClients:
  {
    system("cls");
    ShowAllClientsScreen();
    GoBackToMainMenue();
    break;
  }
  case enMainMenueOptions::eAddNewClient:
    system("cls");
    ShowAddNewClientsScreen();
    GoBackToMainMenue();
    break;

  case enMainMenueOptions::eDeleteClient:
    system("cls");
    ShowDeleteClientScreen();
    GoBackToMainMenue();
    break;

  case enMainMenueOptions::eUpdateClient:
    system("cls");
    ShowUpdateClientScreen();
    GoBackToMainMenue();
    break;

  case enMainMenueOptions::eFindClient:
    system("cls");
    ShowFindClientScreen();
    GoBackToMainMenue();
    break;

  case enMainMenueOptions::eShowTransactionsMenue:
    system("cls");
    ShowTransactionsMenue();
    break;

  case enMainMenueOptions::eExit:
    system("cls");
    ShowEndScreen();
    break;
  }
}

/** @brief すべてのクライアントの画面を表示する
	* @details この関数は、現在接続されているすべてのクライアントの画面を表示します。各クライアントの画面は、それぞれ独立して表示されます。この関数を呼び出すと、すべてのクライアントの画面が一覧できます。具体的な表示方法やレイアウトは、実装に依存します。
	*/
void ShowMainMenue()
{
  system("cls");
  cout << "===========================================\n";
  cout << "\t\tMain Menue Screen\n";
  cout << "===========================================\n";
  cout << "\t[1] Show Client List.\n";
  cout << "\t[2] Add New Client.\n";
  cout << "\t[3] Delete Client.\n";
  cout << "\t[4] Update Client Info.\n";
  cout << "\t[5] Find Client.\n";
  cout << "\t[6] Transactions.\n";
  cout << "\t[7] Exit.\n";
  cout << "===========================================\n";
  PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}

/** @brief メイン関数
	* @return int プログラムの終了ステータスを返します。正常終了の場合は0を返します。
	* @details この関数はプログラムのエントリーポイントです。ここからプログラムの実行が開始されます。
	*/
int main()
{
  ShowMainMenue();
  system("pause>0");
  return 0;
}