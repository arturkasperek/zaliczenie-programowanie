#include <iostream>
#include <string>
#include <curl/curl.h>
#include "server_http.hpp"
#include "CinkciarzValidator.h"

using namespace std;

int main() {
	CinkciarzValidator::getCurrencyArray();
	system("PAUSE");

	return 0;
}