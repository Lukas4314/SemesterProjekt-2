#include "StockfishEngine.h"

StockfishEngine::StockfishEngine(const std::string& path) : enginePath(path), engine(nullptr) {}

StockfishEngine::~StockfishEngine() {
    if (engine) {
        PCLOSE(engine);
    }
}

bool StockfishEngine::start() {
    engine = POPEN(enginePath.c_str(), "w");
    return engine != nullptr;
}

std::string StockfishEngine::sendCommand(const std::string& command) {
    if (!engine) return "Error: Engine not running.";

    fprintf(engine, "%s\n", command.c_str());
    fflush(engine);

    char buffer[256];
    std::string response;

    while (fgets(buffer, sizeof(buffer), engine)) {
        response += buffer;
        if (std::string(buffer).find("bestmove") != std::string::npos) {
            break;
        }
    }
    return response;
}
