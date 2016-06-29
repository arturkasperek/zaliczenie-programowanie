#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <boost/algorithm/string.hpp>

#include "Server.h"
#include "CinkciarzValidator.h"
#include "DataService.h"
#include "Transaction.h"

using namespace std;

Currency getSymbolCurrencyObject(vector<Currency> currencies, string currencyName) {
	for (auto &currency : currencies)
	{
		if (currency.getSymbol() == currencyName) {
			return currency;
		}
	}
	return currencies[0];
}

int main() {
//	auto currencies = CinkciarzValidator::validDataToCurrencyObject(DataService::getDataFromUrl("https://cinkciarz.pl/currencies/index/5/json"));
//	auto test = currencies[1].getRates()->at(0);
	auto server = new Server();
	ofstream transactionsFile;
	//sellCurrency(0, 10);

	server->get("currencies", [](Response& response, std::shared_ptr<Request> request){
		//auto content = request->content.string() + "dziala";
		string currencyJSONString = DataService::getDataFromUrl("https://cinkciarz.pl/currencies/index/5/json");

		response << "HTTP/1.1 200 OK\r\nContent-Length: " << currencyJSONString.length() << "\r\n\r\n" << currencyJSONString;
	});

	server->post("addCurrencyTransaction", [&transactionsFile](Response& response, std::shared_ptr<Request> request) {
		std::string responseText = "",
					requestTransactionSerialize = request->content.string();

		transactionsFile.open("file_db/transactions.txt", ios_base::app);
		transactionsFile << requestTransactionSerialize << "\n";
		transactionsFile.close();

		response << "HTTP/1.1 200 OK\r\nContent-Length: " << responseText.length() << "\r\n\r\n" << responseText;
	});

	server->post("sellCurrency", [](Response& response, std::shared_ptr<Request> request){
	    string  responseText = "",
	            sellCurrencyInfo = request->content.string();
        vector<string> splittedSellTransaction;
        boost::split(splittedSellTransaction, sellCurrencyInfo, boost::is_any_of("\t "));
        sellCurrency(stoi(splittedSellTransaction[0]), stoi(splittedSellTransaction[1]), stof(splittedSellTransaction[2]), stof(splittedSellTransaction[3]));
        cout << splittedSellTransaction[0] << " " << splittedSellTransaction[1] << endl;
        //int id, int amount, int sellPrice, int buyPrice
	    cout << sellCurrencyInfo << endl;
	    response << "HTTP/1.1 200 OK\r\nContent-Length: " << responseText.length() << "\r\n\r\n" << responseText;
	});

	server->get("transactions", [&transactionsFile](Response& response, std::shared_ptr<Request> request) {
		std::string transactionsJSArray = "{\"transactions\": [";
		std::string transactionSerialize;
		ifstream file("file_db/transactions.txt");
		bool isFileEmpty = file.peek() == std::ifstream::traits_type::eof() ? true : false;
		auto currencies = CinkciarzValidator::validDataToCurrencyObject(DataService::getDataFromUrl("https://cinkciarz.pl/currencies/index/5/json"));
		float transactionsSumProfit = 0;

		while (getline(file, transactionSerialize))
		{
			auto transaction = new Transaction(transactionSerialize);
			auto currency = getSymbolCurrencyObject(currencies, transaction->getCurrencySymbol());
			auto transactionProfit = transaction->calcActualTransactionState(currency.getRates());
			transactionsSumProfit += transactionProfit;
			//cout << transactionProfit << "\n";
			//cout << transaction->getCurrencySymbol() << "\n";
			transactionsJSArray += "\"" + transactionSerialize + " " + to_string(transactionProfit) + "\",";
		}
		if(!isFileEmpty) transactionsJSArray.pop_back(); //we must delete last ','
		transactionsJSArray += "], \"transactionsSumProfit\": " + to_string(transactionsSumProfit) + "}";
		file.close();
		response << "HTTP/1.1 200 OK\r\nContent-Length: " << transactionsJSArray.length() << "\r\n\r\n" << transactionsJSArray;
	});

    server->get("sellHistory", [](Response& response, std::shared_ptr<Request> request) {
        std::ifstream sellHistoryFile("file_db/sell_history.txt");
        string sellHistory((istreambuf_iterator<char>(sellHistoryFile)), istreambuf_iterator<char>());
        response << "HTTP/1.1 200 OK\r\nContent-Length: " << sellHistory.length() << "\r\n\r\n" << sellHistory;
    });

	server->serveStatic("static");

#ifdef __linux__
	//std::system("google-chrome-stable http://localhost:8080/");
#elif _WIN32
	std::system("start chrome http://localhost:8080/");
#endif

	server->start();
	return 0;
}