#include "ur5_sem2_scripts/Logger.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <filesystem>

namespace fs = std::filesystem;

std::ofstream Logger::logFile;
std::string Logger::fileName;
std::vector<std::string> Logger::buffer;
std::vector<std::string> Logger::headers;
std::unordered_map<std::string, size_t> Logger::headerIndexMap;

std::string Logger::currentDateTimeForFile() {
    time_t now = time(0);
    struct tm tstruct = *localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tstruct, "%Y-%m-%d_%H-%M-%S");
    return oss.str();
}

std::string Logger::generateUniqueFileName() {
    std::string baseName = "log_" + currentDateTimeForFile();
    std::string finalName;
    int counter = 0;

    do {
        std::ostringstream oss;
        oss << baseName;
        if (counter > 0)
            oss << "_" << counter;
        oss << ".csv";
        finalName = oss.str();
        counter++;
    } while (fs::exists(finalName));

    return finalName;
}

std::string Logger::escapeCsv(const std::string& s) {
    std::string result;
    for (char c : s) {
        if (c == '"') {
            result += "\"\"";
        } else {
            result += c;
        }
    }
    return result;
}

void Logger::writeHeaders() {
    for (size_t i = 0; i < headers.size(); ++i) {
        logFile << "\"" << escapeCsv(headers[i]) << "\"";
        if (i < headers.size() - 1) {
            logFile << ",";
        }
    }
    logFile << std::endl;
}

void Logger::initialize(const std::vector<const char*>& headerNames) {
    fileName = generateUniqueFileName();
    logFile.open(fileName, std::ios::out);
    if (!logFile.is_open()) {
        std::cerr << "Failed to create log file: " << fileName << std::endl;
    } else {
        headers.clear();
        headerIndexMap.clear();

        for (const auto& name : headerNames) {
            headers.push_back(name);
            headerIndexMap[name] = headers.size() - 1;
        }

        buffer.assign(headers.size(), "");
        writeHeaders();
        std::cout << "Logger initialized: " << fileName << std::endl;
    }
}

void Logger::setValue(const char* headerName, const std::string& value) {
    auto it = headerIndexMap.find(headerName);
    if (it != headerIndexMap.end()) {
        buffer[it->second] = value;
    } else {
        std::cerr << "Unknown header name: " << headerName << std::endl;
    }
}

void Logger::writeRow() {
    if (logFile.is_open()) {
        for (size_t i = 0; i < buffer.size(); ++i) {
            logFile << "\"" << escapeCsv(buffer[i]) << "\"";
            if (i < buffer.size() - 1) {
                logFile << ",";
            }
        }
        logFile << std::endl;
        buffer.assign(headers.size(), "");
    }
}

void Logger::close() {
    if (logFile.is_open()) {
        logFile.close();
    }
}
