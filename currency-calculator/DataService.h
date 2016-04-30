#pragma once
#pragma comment(lib, "urlmon.lib")
#include <urlmon.h>
#include <tchar.h>
#include <string>
#include <fstream>

class DataService {
private:
	static void saveDataFromUrlToFile(std::string url, std::string filePath) {
		std::string pathToSaveFile;
		TCHAR path[MAX_PATH];

		GetCurrentDirectory(MAX_PATH, path);

		pathToSaveFile = path;
		pathToSaveFile += filePath;

		HRESULT res = URLDownloadToFile(NULL, "https://cinkciarz.pl/currencies/index/5/json", pathToSaveFile.c_str(), 0, NULL);
	}

	static std::string getDataFromFile(std::string filePath) {
		std::ifstream t(filePath);
		std::stringstream buffer;
		buffer << t.rdbuf();

		return buffer.str();
	}
public:
	static std::string getDataFromUrl(std::string url) {
		saveDataFromUrlToFile(url, "\\cinkciarzData.json");
		return getDataFromFile("cinkciarzData.json");
	}
};
