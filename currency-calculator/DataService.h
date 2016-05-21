#pragma once

#ifdef __linux__
	#include <curl/curl.h>
#elif _WIN32
	#pragma comment(lib, "urlmon.lib")
	#include <urlmon.h>
#endif

#include <boost/filesystem.hpp>
#include <string>
#include <fstream>

class DataService {
private:
#ifdef __linux__
	static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
	{
		((std::string*)userp)->append((char*)contents, size * nmemb);
		return size * nmemb;
	}

	static std::string getDataFromUrlWithCurl(std::string url) {
		CURL *curl;
		CURLcode res;
		std::string readBuffer;

		curl = curl_easy_init();
		if (curl) {
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
			res = curl_easy_perform(curl);
			curl_easy_cleanup(curl);
		}
		return readBuffer;
	}
#elif _WIN32
	static void saveDataFromUrlToFile(std::string url, std::string filePath) {
		std::string pathToSaveFile;
		boost::filesystem::path current_path( boost::filesystem::current_path() );

		pathToSaveFile = current_path.string() + filePath;

		HRESULT res = URLDownloadToFile(NULL, "https://cinkciarz.pl/currencies/index/5/json", pathToSaveFile.c_str(), 0, NULL);
	}

	static std::string getDataFromFile(std::string filePath) {
		std::ifstream t(filePath);
		std::stringstream buffer;
		buffer << t.rdbuf();

		return buffer.str();
	}
#endif

public:
	static std::string getDataFromUrl(std::string url) {
#ifdef __linux__
		return getDataFromUrlWithCurl(url);
#elif _WIN32
		saveDataFromUrlToFile(url, "\\cinkciarzData.json");
		return getDataFromFile("cinkciarzData.json");
#endif
	}
};
