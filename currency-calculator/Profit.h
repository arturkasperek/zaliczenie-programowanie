#pragma once
#include <vector>
#include <string>
#include "Currency.h"
#include "Transaction.h"

struct Profit : public Currency{
	float profit;
	float overallAmount;

	Profit(string pSymbol, string pName, vector<Rate>* pRates) : Currency(pSymbol, pName, pRates) {profit=0; overallAmount=0;}
	Profit operator+=(Transaction &transaction);
};

Profit Profit::operator+=(Transaction &transaction)
{
	if(getSymbol()==transaction.getCurrencySymbol())
	{
		profit+=transaction.calcActualTransactionState(getRates());
		overallAmount+=transaction.getAmount();
		return *this;
	}
	else throw "profit::operator+=: Currencies do not match!";
}
Profit getSymbolCurrencyObject(vector<Profit> currencies, string currencyName)
{
	for(auto &currency : currencies) if(currency.getSymbol() == currencyName) return currency;
	return currencies[0];
}
