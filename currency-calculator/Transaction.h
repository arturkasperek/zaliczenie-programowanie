#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include "Currency.h"
using namespace std;

class Transaction {
private:
	std::string currencySymbol;
	float amount;
	float course;
public:
	Transaction(std::string pCurrencyName, float pAmount, float pCourse) {
		currencySymbol = pCurrencyName;
		amount = pAmount;
		course = pCourse;
	}
	
	Transaction(std::string serializeObject) {
		vector<string> splittedTransaction;
		boost::split(splittedTransaction, serializeObject, boost::is_any_of("\t "));
		currencySymbol = splittedTransaction[0];
		amount = stof(splittedTransaction[1]);
		course = stof(splittedTransaction[2]);
	}

	string getCurrencySymbol() {
		return currencySymbol;
	}

	float getSalePrice(vector<Rate>* rates) {
		int searchAmount = amount <= 10 ? 10: 1000;
		searchAmount = amount <= 50000 ? 50000 : searchAmount;

		for (int x = 0; x < 3; x++) {
			Rate rate = rates->at(x);

			if ( rate.amount == searchAmount) {
				return rate.salePrice;
			}
		}
	}

	float calcActualTransactionState(vector<Rate>* actualCurrencyRates) {
		return (amount * getSalePrice(actualCurrencyRates)) - (amount * course);
	}
};

void sellCurrency(int id, int amount, float sellPrice, float buyPrice){
    string line;
    int currId = 0;
    fstream myfile ("file_db/transactions.txt");
    fstream sellHistoryFile;
    string newFile = "";
    vector<string> splittedTransaction;
    while ( getline (myfile, line) )
    {
        if(currId == id) {
             boost::split(splittedTransaction, line, boost::is_any_of("\t "));
             int newCurrencyAmount = stof(splittedTransaction[1]) - amount;
             newFile += splittedTransaction[0] + " " + to_string(newCurrencyAmount) + " " + splittedTransaction[2] + '\n';
             currId++;
        } else {
            newFile += line + '\n';
        }
        currId++;
    }

    cout << newFile;
    myfile.close();
    myfile.open("file_db/transactions.txt", std::fstream::out | std::fstream::trunc);
    myfile << newFile;
    myfile.close();


    sellHistoryFile.open("file_db/sell_history.txt", ios_base::app);
    sellHistoryFile << splittedTransaction[0] << " " << to_string(amount) << " " << to_string(sellPrice) << " " <<to_string(buyPrice) << "\n";
    sellHistoryFile.close();
}