#ifndef STOCKFISH_ENGINE_H
#define STOCKFISH_ENGINE_H

#include <iostream>
#include <string>
#include <cstdio>

#ifdef _WIN32
#define POPEN _popen
#define PCLOSE _pclose
#else
#define POPEN popen
#define PCLOSE pclose
#endif

class StockfishEngine {
private:
    std::string enginePath;
    FILE* engine;

public:
    StockfishEngine(const std::string& path);
    ~StockfishEngine();
    bool start();
    std::string sendCommand(const std::string& command);
};

#endif
