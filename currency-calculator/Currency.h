#pragma once
#include <string>
#include <iostream>
using namespace std;

struct Rate{
	float salePrice;
	float buyPrice;
	int amount;
};

class Currency{
	string symbol;
	string name;
	vector<Rate>* rates;
public:
	Currency(string pSymbol, string pName, vector<Rate>* pRates) : symbol(pSymbol), name(pName), rates(pRates) {}
	vector<Rate>* getRates() {return rates;}
	string getSymbol() {return symbol;}
	string getName() {return name;}
};

Currency getSymbolCurrencyObject(vector<Currency> currencies, string currencyName)
{
	for(auto &currency : currencies) if(currency.getSymbol() == currencyName) return currency;
	return currencies[0];
}
