#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <iostream>
#include <regex>
#include <boost/filesystem.hpp>

#include "server_http.hpp"

typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;
typedef HttpServer::Response Response;
typedef HttpServer::Request Request;

using namespace std;
using namespace boost;

class Server {
private:
	HttpServer * server;
public:
	Server() {
		server = new HttpServer(8080);
	}
	void start() {	
		thread server_thread([&server = server]() {
			server->start();
		});

		this_thread::sleep_for(chrono::seconds(1));

		server_thread.join();
	}

	void serveStatic(std::string pathToStatic) {
		std::regex indexRequestRegex("/$");

		server->default_resource["GET"] = [pathToStatic, indexRequestRegex](Response& response, std::shared_ptr<Request> request) {
			auto requestPath = request->path;

			if (std::regex_match(requestPath, indexRequestRegex)) {
				requestPath += "index.html";
			}

			auto resourceUrl = pathToStatic + requestPath;

			ifstream resourceStream(resourceUrl.c_str());

			if (resourceStream.good()) {
				std::stringstream buffer;
				string content;

				buffer << resourceStream.rdbuf();
				content = buffer.str();

				response << "HTTP/1.1 200 OK\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
			}
			else {
				string content = "Could not open path " + request->path;
				response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
			}
			return;
		};
	}

	template <class T> 
	void get(std::string route, T&& callback) {
		server->resource["^/" + route + "$"]["GET"] = callback;
	}

	template <class T>
	void post(std::string route, T&& callback) {
		server->resource["^/" + route + "$"]["POST"] = callback;
	}
};