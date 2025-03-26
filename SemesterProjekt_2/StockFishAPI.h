#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <nlohmann/json.hpp> // Include the JSON library downloaded from https://github.com/nlohmann/json/tree/develop/include/nlohmann
#include <fstream>  // For file handling

using namespace std;

class StockfishAPI {
public:
    // Function to URL-encode a string, This function isnt super important, however it is essential in safe web data transfer
    static string urlEncode(const string& value) { 
        ostringstream escaped;
        escaped.fill('0');
        escaped << hex;

        for (char c : value) {
            if (isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_' || c == '.' || c == '~') {
                escaped << c;
            }
            else {
                escaped << '%' << setw(2) << int(static_cast<unsigned char>(c));
            }
        }
        return escaped.str();
    }

    // This function acts like a coded command window (or terminal) inside our C++ program. It allows us to execute shell commands dynamically and capture the output as a string.
    static string exec(const char* cmd) {
        char buffer[128];
        string result = "";

        FILE* pipe = popen(cmd, "r");
        if (!pipe) throw runtime_error("popen failed!");
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }
        pclose(pipe);
        return result;
    }

    // Function to write the analysis response to a JSON file
    static void writeJsonToFile(const nlohmann::json& jsonData, const string& filePath) {
        ofstream file(filePath);
        if (file.is_open()) {
            file << jsonData.dump(4);  // Pretty print with 4 spaces of indentation
            file.close();
            cout << "JSON data written to " << filePath << endl;
        }
        else {
            cerr << "Failed to open the file for writing!" << endl;
        }
    }

    // Function to analyze the chess position using Stockfish and store the result in a JSON file
    static void analyzePosition(const string& fen, int depth) {
        string encodedFen = urlEncode(fen);
        string url = "https://stockfish.online/api/s/v2.php?fen=" + encodedFen + "&depth=" + to_string(depth);

        string command = "curl -X GET \"" + url + "\" --ssl-no-revoke";

        // Execute the curl command and capture the response
        string response = exec(command.c_str());

        // Output the raw response for debugging purposes
        cout << "Stockfish Analysis Response: " << response << endl;

        // Parse the response into a JSON object
        nlohmann::json jsonResponse;
        try {
            jsonResponse = nlohmann::json::parse(response);
        }
        catch (const nlohmann::json::parse_error& e) {
            cerr << "Failed to parse response as JSON: " << e.what() << endl;
            return;
        }

        // Write the JSON response to a file
        string filePath = "stockfish_analysis.json";  // Specify the file path of where the output JSON should be
        writeJsonToFile(jsonResponse, filePath);
    }
};

