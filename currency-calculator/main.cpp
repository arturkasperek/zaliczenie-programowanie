#include <iostream>
#include <string>
#include <curl/curl.h>
#include "DataService.h"
#include "server_http.hpp"

using namespace std;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

int main() {
	std::string val= DataService::getDataFromUrl("https://www.google.pl/");
	cout << val;
	system("PAUSE");

	return 0;
}