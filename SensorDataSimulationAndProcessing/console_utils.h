#ifndef CONSOLE_UTILS_H
#define CONSOLE_UTILS_H

#include <mutex>
#include <iostream>
#include <vector>
#include <sstream>

// Global mutex for console printing
extern std::mutex printMutex;

// Function prototypes
void clearConsole();
void moveCursorTo(int row, int col);
void printInRegion(int startCol, int startRow, int endRow, const std::string& content);
void resetCursorToCommandRegion(int commandRow);
void clearLine(int row);
std::vector<std::string> splitStringByNewline(const std::string& str);

#endif // CONSOLE_UTILS_H