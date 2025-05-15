#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>

// Define your header constants here
constexpr const char *HALF_MOVES = "HalfMoves";
constexpr const char *CAMERA_MOVE = "CameraMove";
constexpr const char *FPS = "FPS";

class Logger
{
private:
    static std::ofstream logFile;
    static std::string fileName;
    static std::vector<std::string> buffer;
    static std::vector<std::string> headers;
    static std::unordered_map<std::string, size_t> headerIndexMap;

    static std::string currentDateTimeForFile();
    static std::string generateUniqueFileName();
    static std::string escapeCsv(const std::string &s);

    static void writeHeaders();

public:
    // Must call first with your desired header constants (array of const char*)
    static void initialize(const std::vector<const char *> &headerNames);

    // Set value by header constant (which is a const char* known at compile time)
    static void setValue(const char *headerName, const std::string &value);

    // Write the buffer row
    static void writeRow();

    // Optional cleanup
    static void close();
};
