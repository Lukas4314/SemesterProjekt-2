/*In this file we have implemented the StockfishUCI class. UCI stands for Universal Chess Interface, and is
a sort of language that the stockfish engine understands. So in order to talk to stockfish we must implement
a class that speaks its language.
Previously we had implemented a stockfishAPI interface, this method is superior in every way. Evaluates faster,
we are using the full power of stockfish, better security because we arent sending over net (earlier we had 
a function for URL encoder), allows better costumization, and lastly it works offline.*/

// Safeguards, assures that there are minimal errors, by making it so that the program cant be included twice in any other implementations
#ifndef STOCKFISH_UCI_H
#define STOCKFISH_UCI_H

#include <iostream>     // For printing to console
#include <string>   // Includes the data type string
#include <stdexcept>    // Used to detect errors
#include <unistd.h>     // IMPORTANT: Here we include various different functions for writing to files etc. A lot of what we use in the code isnt c++ features, instead we use the linux functionallity to communicate directly with the OS (Operating System)
#include <sys/types.h>   // Later we use pid_t as a data type that saves an ID. This comes from this lib
#include <sys/wait.h>   // Library used for waiting (Delays)
#include "rclcpp/rclcpp.hpp" // For logging

using namespace std;

// Class for fetching best move based on current position given in the FEN notation
class StockfishUCI {
private:
    int toEngine[2];   // Parent writes to child. For this implementation we are writing to the stockfish engine. In order to do so we use "pipes", imagine a pipe to the stockfish engine. Through this pipe we send a 2 integer array which contains important info that the UCI needs.
    int fromEngine[2];  // Parent reads from child. This is the same but we use this to recieve info from the stockfish engine
    pid_t childPid;     // pid_t is a data type (Process ID Type). This is a number used to identify a running program in the system. And we use the variable childPid to track this data

public:
    // Constructer which runs instantly when a new stockfishUCI object is created
    StockfishUCI(const string& path = "/usr/games/stockfish") {     // This constructer takes a path to the stockfish engine. If nothing is declared it defaults to the defined path
        if (pipe(toEngine) == -1 || pipe(fromEngine) == -1) {   // Here we use the pipe function from the unistd.h class. Pipe allows us to create two new pipes which we are going to use to communicate with the stockfish engine. 
            throw runtime_error("Failed to create pipes");      // We are in this step checking to see if the pipes fail (return -1). If they do we print an error message
        }

        childPid = fork();  // IMPORTANT: This is a pretty important line. We use the fork() function from the unistd.h class. We use it to create a child program. It works like this. The function returns any positive number = you are the parent. The function returns 0 = you are the child. The function returns any negative number = error. If a positive number is returned it is the childs process ID
        if (childPid == -1) {   // So this is again just error checking
            throw runtime_error("Failed to fork process");  // Here we use the stdexcept functionality
        }
        
        // NOTE: After fork() the code "splits in two" and runs from line 28. This next if block splits the program in two one the child if = 0, and the other parent if child not = 0
        if (childPid == 0) {    // Checks the variable childPid which tells us wether or not it is the child or the parent program. In this case we choose/check for the child.
            // Child process. So for the child process we are trying to create stockfish and let it run in the background.
            dup2(toEngine[0], STDIN_FILENO);   // In the stockfish part of the program we replace the input pipe with the read part of the to engine pipe. We do this so that the child (Stockfish) reads info from the main program. We do this by toEngine[0] where 0 = standard input (keyboard), 1 = standard output (terminal), 2 = standard error (error output)
            dup2(fromEngine[1], STDOUT_FILENO); // Again we make it so that the child (Stockfish) replaces the output with our write pipe. So when stockfish prints anything it goes into this pipe 
            close(toEngine[1]);     // Here we close the unused ends of the pipes. We simply dont need to use toEngine pipe to write to stockfish. 
            close(fromEngine[0]);   // We also dont need to use the fromEngine pipe to read

            execl(path.c_str(), path.c_str(), (char*)nullptr);  // Here we basically initiate the child to be stockfish by giving it the correct paths. If this line runs the next line will never run meaning we dont exit in line 43
            perror("execl failed");     // For error handling which could occur if we dont get acces to stockfish
            exit(1);    // Then exits the code after error so that it doesnt keep running code its not supposed to. Only executes if execl fails
        } else {    // This is ofcourse how we initiate the parent process
            // Parent process. We dont need to use dup2 to redine the pipes and the way they work because we just use the default values
            close(toEngine[0]);     // The parent writes toEngine[1] so we close toEngine[0] because we dont need to write anything through this pipe
            close(fromEngine[1]);   // The parent closes fromEngine[1] because we dont need to use this pipe to write anything only read by toEngine[0] 
            cout << "[UCI] Stockfish started with PID " << childPid << endl;    // Here we print a confirmation message along with childPid which is the child ID. Mainly used for debugging/confermation
        }
    }

    // The destructer which runs everytime the stockFishUCI object goes out of scope (finishes)
    ~StockfishUCI() {
        if (childPid > 0) {     // Makes sure we are on the parent (only the parent does the cleanup)
            writeCommand("quit");   // We use the writeCommand function with the keyword quit to shutdown the child
            close(toEngine[1]);     // Closes the toEngine pipe fully
            close(fromEngine[0]);   // Closes the fromEngine pipe fully
            waitpid(childPid, nullptr, 0);      // Waits for the child to exit
        }
    }

    // Function that we can use to write a command to stockfish. It takes a string cmd
    void writeCommand(const string& cmd) {
        string line = cmd + "\n";   // We make a new variable "line" which is a string that consists of the input given to the function and \n which is clasic notation used for switching line. Stockfish expects this syntax and because this function writes to stockfish we need it 
        write(toEngine[1], line.c_str(), line.length());    // Because we use toEngine[1] we are writing from the parent to stockfish. We are sending the "line" variable, and we also need to provide the write function with the length of "line". We just use a built in length function
    }

    // Function for reading output from stockfish until a keyword is passed to the function
    string readOutputUntil(const string& token) {
        string output;  // A string variable to store all the output from stockfish
        char buffer[256];   // Temporary char (character) array to hold chuncks of data coming from stockfish
        while (true) {      // Initiates while true loop
            ssize_t count = read(fromEngine[0], buffer, sizeof(buffer) - 1);    // Low level system call which reads data fromEngine[0] which would be the end of the pipe that the parent uses.
            if (count <= 0) break;  // If we get less than 0 or nothing (so 0) from stockfish break this loop
            buffer[count] = '\0';   // Adds \0 to buffer so that we can safely handle it as a string
            output += buffer;   // Append the data we just read and added to the buffer into our output variable
            if (output.find(token) != string::npos) break;  // Checks if the "token" so bestmove for instance is inside output. If it is we exit the loop. Later we can then read the best move
        }
        return output;  // Function returns output
    }

    // Function for getting the best move.
    string getBestMove(const string& moves, int depth = 15) {       // Sends two things. A string of moves (e2e4, a3a4 etc.) and an integer depth which is the amount of steps ahead stockfish should be
        writeCommand("uci");    // Tells stockfish "Lets start a convo using UCI (Universal Chess Interface)"
        readOutputUntil("uciok");   // This sets up the protocal basically makes sure that stockfish is ready

        writeCommand("isready");    // Now we ask if stockfish is ready
        readOutputUntil("readyok");     // We read until it tells us it is ready

        writeCommand("ucinewgame");     // Start a new game (Resets everything)
        writeCommand("position startpos moves " + moves);   // Tells stockfish to start from the start position "position startpos moves" and then to one after the other apply everything from the string moves
        writeCommand("go depth " + to_string(depth));   // This is the actual search command which tells stockfish to then calculate the bestmove based on the depth
        RCLCPP_DEBUG(rclcpp::get_logger("StockfishUCI"), "Moves are: %s", moves.c_str());
        return readOutputUntil("bestmove");     // lastly we read until best move keyword is found at which point a string is returned by the function
    }

    string getBestMoveFromFen(const string& fen, const string& moves = "", int depth = 15) {
        writeCommand("uci");
        readOutputUntil("uciok");
    
        writeCommand("isready");
        readOutputUntil("readyok");
    
        writeCommand("ucinewgame");
        writeCommand("position fen " + fen + " moves " + moves);
        writeCommand("go depth " + to_string(depth));
    
        return readOutputUntil("bestmove");
    }
    
};

#endif // STOCKFISH_UCI_H
