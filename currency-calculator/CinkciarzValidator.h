#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "jsonxx.h"
#include "DataService.h"
#include "Currency.h"

using namespace jsonxx;
using namespace std;

class CinkciarzValidator {
private:
	static vector<Currency> validDataToCurrencyObject(string dataToValid) {
		Object currencyJSON;
		Array currencyArray;
		vector <Currency> currencyObjectsVector;

		currencyJSON.parse(dataToValid);

		currencyArray = currencyJSON.get<Array>("currencies");
		for (int i = 0; i < (int)currencyArray.size(); i++) {
			Object currency = currencyArray.get<Object>(i).get<Object>("Currency"),
				rateA = currency.get<Object>("ratesA"),
				rateB = currency.get<Object>("ratesB"),
				rateC = currency.get<Object>("ratesC");
			string symbol = currency.get<String>("name");
			string fullName = currency.get<String>("full_name");
			Rate rates[] = {
				{ (float)rateA.get<Number>("1"), (float)rateA.get<Number>("2"), (int)rateA.get<Number>("amount") },
				{ (float)rateB.get<Number>("1"), (float)rateB.get<Number>("2"), (int)rateB.get<Number>("amount") },
				{ (float)rateC.get<Number>("1"), (float)rateC.get<Number>("2"), (int)rateC.get<Number>("amount") }
			};

			currencyObjectsVector.push_back(Currency(symbol, fullName, rates));
		}

		return currencyObjectsVector;
	}
public:
	static vector<Currency> getCurrencyArray() {
		string currencyJSONString;

		currencyJSONString = DataService::getDataFromUrl("https://cinkciarz.pl/currencies/index/5/json");

		return validDataToCurrencyObject(currencyJSONString);
	}
};