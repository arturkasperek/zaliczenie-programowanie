#include <iostream>
#include <string>
#include "Server.h"
#include "CinkciarzValidator.h"
#include "DataService.h"

using namespace std;

int main() {

	string currencyJSONString;

	currencyJSONString = DataService::getDataFromUrl("https://cinkciarz.pl/currencies/index/5/json");

	auto currencies = CinkciarzValidator::validDataToCurrencyObject(currencyJSONString);

	auto server = new Server();

	server->get("currencies", [currencyJSONString](Response& response, std::shared_ptr<Request> request){
		//auto content = request->content.string() + "dziala";

		response << "HTTP/1.1 200 OK\r\nContent-Length: " << currencyJSONString.length() << "\r\n\r\n" << currencyJSONString;
	});

	server->post("add_currency_purchase", [](Response& response, std::shared_ptr<Request> request) {
		std::string test = "work ok :)";
		response << "HTTP/1.1 200 OK\r\nContent-Length: " << test.length() << "\r\n\r\n" << test;
	});

	server->serveStatic("static");

	std::system("start chrome http://localhost:8080/");

	server->start();


	return 0;
}