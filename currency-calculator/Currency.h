#pragma once
#include <string>
#include <tuple>
#include <iostream>
using namespace std;

struct Rate{
	float salePrice;
	float buyPrice;
	int amount;
};

class Currency {
private:
	string symbol;
	string name;
	Rate* rates;
public:
	Currency(string pSymbol, string pName, Rate* pRates) {
		symbol = pSymbol;
		name = pName;
		rates = pRates;
	}
};