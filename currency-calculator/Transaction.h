#pragma once
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class Transaction {
private:
	std::string currencyName;
	float amout;
	float course;
public:
	Transaction(std::string pCurrencyName, float pAmout, float pCourse) {
		currencyName = pCurrencyName;
		amout = pAmout;
		course = pCourse;
	}
	
	Transaction(std::string serializeObject) {

	}

	bool saveToFile(std::string fileName) {
		/*
		ofstream transactionsFile;

		transactionsFile.open(fileName, ios_base::app);
		transactionsFile << currencyName << " " << to_string(amout) << " " << to_string(course) << "\n";

		return true;*/
	}
};