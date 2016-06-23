#include <iostream>
#include <string>
#include <fstream>
#include "Server.h"
#include "CinkciarzValidator.h"
#include "DataService.h"
#include "Transaction.h"
using namespace std;
string trunc_f(string s, int dec_pl=2)
{
	int i=0;
	while(i<s.size() && s[i]!='.') ++i;
	if(dec_pl!=0) i+=1+dec_pl;
	if(i<s.size()) return s.substr(0,i);
	return s;
}
int main() {
//	auto currencies = CinkciarzValidator::validDataToCurrencyObject(DataService::getDataFromUrl("https://cinkciarz.pl/currencies/index/5/json"));
//	auto test = currencies[1].getRates()->at(0);
	auto server = new Server();
	ofstream transactionsFile;

	server->get("currencies", [](Response& response, std::shared_ptr<Request> request)
	{
		//auto content = request->content.string() + "dziala";
		string currencyJSONString = DataService::getDataFromUrl("https://cinkciarz.pl/currencies/index/5/json");

		response << "HTTP/1.1 200 OK\r\nContent-Length: " << currencyJSONString.length() << "\r\n\r\n" << currencyJSONString;
	});

	server->post("addCurrencyTransaction", [&transactionsFile](Response& response, std::shared_ptr<Request> request)
	{
		std::string responseText = "",
					requestTransactionSerialize = request->content.string();

		transactionsFile.open("file_db/transactions.txt", ios_base::app);
		transactionsFile << requestTransactionSerialize << "\n";
		transactionsFile.close();

		response << "HTTP/1.1 200 OK\r\nContent-Length: " << responseText.length() << "\r\n\r\n" << responseText;
	});

	server->get("transactions", [&transactionsFile](Response& response, std::shared_ptr<Request> request)
	{
		std::string transactionsJSArray = "{\"transactions\": [", transactionSerialize;
		ifstream file("file_db/transactions.txt");
		bool isFileEmpty = file.peek() == std::ifstream::traits_type::eof() ? true : false;
		vector<Profit> currencies=validDataToCurrencyObject(DataService::getDataFromUrl("https://cinkciarz.pl/currencies/index/5/json"));
		float transactionsSumProfit;
		while(getline(file, transactionSerialize)) //getting currencies we posses; counting profit
		{
			auto transaction = new Transaction(transactionSerialize);
			auto currency = getSymbolCurrencyObject(currencies, transaction->getCurrencySymbol());
			for(int i=0; i<currencies.size(); ++i) if(currencies[i].getSymbol() == transaction->getCurrencySymbol())
			{
				currencies[i]+=*transaction;
				delete transaction;
				break;
			}
		}
		for(int i=0; i<currencies.size(); ++i) if(currencies[i].overallAmount != 0)
		{
			transactionsSumProfit += currencies[i].profit;
			cout << currencies[i].getSymbol() << " " << currencies[i].profit << endl;
			transactionsJSArray += "\"" + currencies[i].getSymbol() + " " + trunc_f(to_string(currencies[i].overallAmount)) + " " + trunc_f(to_string(currencies[i].getRates()[0][0].salePrice)) + " " + trunc_f(to_string(currencies[i].profit)) + "\",";
		}
		if(!isFileEmpty) transactionsJSArray.pop_back(); //we must delete last ','
		transactionsJSArray += "], \"transactionsSumProfit\": " + trunc_f(to_string(transactionsSumProfit), 0) + "}";
		file.close();
		response << "HTTP/1.1 200 OK\r\nContent-Length: " << transactionsJSArray.length() << "\r\n\r\n" << transactionsJSArray;
	});

	server->serveStatic("static");

#ifdef __linux__
	std::system("firefox http://localhost:8080/");
	//std::system("google-chrome-stable http://localhost:8080/");
#elif _WIN32
	std::system("start chrome http://localhost:8080/");
#endif
	server->start();
	return 0;
}
