#include "console_utils.h"
#include <iostream>
#include <thread>



// Clear the console screen
void clearConsole() {
    std::cout << "\033[2J\033[H"; // ANSI escape code: Clear screen and move cursor to top
}

// Move the cursor to a specific row and column
void moveCursorTo(int row, int col) {
    std::cout << "\033[" << row << ";" << col << "H"; // ANSI escape code: Move cursor
}

void clearLine(int row) {
	moveCursorTo(row, 1);
	std::cout << "\033[K"; // Clear the line

}

// Split a string by newline character
std::vector<std::string> splitStringByNewline(const std::string& str) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string line;
    while (std::getline(ss, line)) {
        result.push_back(line);
    }
    return result;
}

// Print content to a specific region with startCol, startRow, endRow parameters
void printInRegion(int startCol, int startRow, int endRow, const std::string& content) {
    std::lock_guard<std::mutex> lock(printMutex); // Lock for thread safety

    // Save cursor position
    std::cout << "\033[s";

    // Clear the region
    for (int row = startRow; row <= endRow; ++row) {

        moveCursorTo(row, startCol);  // Move to the start of each row
        std::cout << "\033[K"; // Clear the line
    }

    // Split the content into lines
    std::vector<std::string> lines = splitStringByNewline(content);

    // Print each line starting at startCol for each row
    int currentRow = startRow;
    for (const auto& line : lines) {
        if (currentRow > endRow) break; // Ensure we don't go beyond the endRow
        moveCursorTo(currentRow, startCol);  // Move cursor to the start of the row and column
        std::cout << line << std::flush; // Print the line
        currentRow++; // Move to the next row
    }

    // Restore cursor position
    std::cout << "\033[u";
    std::cout.flush();
}

// Reset cursor to the command region
void resetCursorToCommandRegion(int commandRow) {
    std::lock_guard<std::mutex> lock(printMutex);
    moveCursorTo(commandRow, 1); // Move cursor to the command row
    std::cout << "> " << std::flush; // Display the command prompt
}