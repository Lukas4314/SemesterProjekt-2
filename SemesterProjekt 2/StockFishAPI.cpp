/*


#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <nlohmann/json.hpp> // Include the JSON library downloaded from https://github.com/nlohmann/json/tree/develop/include/nlohmann
#include <fstream>  // For file handling

// Function to URL-encode a string
std::string urlEncode(const std::string& value) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (char c : value) {
        if (isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
        }
        else {
            escaped << '%' << std::setw(2) << int(static_cast<unsigned char>(c));
        }
    }

    return escaped.str();
}

// Function to execute a shell command and return the output
std::string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";

    FILE* pipe = _popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen failed!");
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    _pclose(pipe);
    return result;
}

// Function to write the analysis response to a JSON file
void writeJsonToFile(const nlohmann::json& jsonData, const std::string& filePath) {
    std::ofstream file(filePath);
    if (file.is_open()) {
        file << jsonData.dump(4);  // Pretty print with 4 spaces of indentation
        file.close();
        std::cout << "JSON data written to " << filePath << std::endl;
    }
    else {
        std::cerr << "Failed to open the file for writing!" << std::endl;
    }
}

// Function to analyze the chess position using Stockfish and store the result in a JSON file
void analyzePosition(const std::string& fen, int depth) {
    std::string encodedFen = urlEncode(fen);
    std::string url = "https://stockfish.online/api/s/v2.php?fen=" + encodedFen + "&depth=" + std::to_string(depth);

    std::string command = "curl -X GET \"" + url + "\" --ssl-no-revoke";

    // Execute the curl command and capture the response
    std::string response = exec(command.c_str());

    // Output the raw response for debugging purposes
    std::cout << "Stockfish Analysis Response: " << response << std::endl;

    // Parse the response into a JSON object
    nlohmann::json jsonResponse;
    try {
        jsonResponse = nlohmann::json::parse(response);
    }
    catch (const nlohmann::json::parse_error& e) {
        std::cerr << "Failed to parse response as JSON: " << e.what() << std::endl;
        return;
    }

    // Write the JSON response to a file
    std::string filePath = "stockfish_analysis.json";  // Specify the file path of where the output JSON should be
    writeJsonToFile(jsonResponse, filePath);
}

int startStockfish() {
    std::string fen = "rn1q1rk1/pp2b1pp/2p2n2/3p1pB1/3P4/1QP2N2/PP1N1PPP/R4RK1 b - - 1 11"; // Example of FEN
    int depth = 15; // Example depth
    analyzePosition(fen, depth);

    return 0;
}


*/