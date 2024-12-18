#include "AOCHelper.h"

#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// 2d vector (x, y)
const std::vector<std::pair<int, int>> directions = {
    {0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1},
};

std::vector<std::vector<char>> splitInput(const std::string &input) {
  std::vector<std::vector<char>> lines;

  std::istringstream lineTokenizer(input);
  std::string tempLineToken;

  while (std::getline(lineTokenizer, tempLineToken)) {
    std::vector<char> characters;
    characters.reserve(tempLineToken.size());
    for (char &c : tempLineToken)
      characters.push_back(c);
    lines.push_back(characters);
  }

  return lines;
}

int countXmasOccurences(const std::vector<std::vector<char>> &wordSearch,
                        int row, int col) {
  const int maxRow = wordSearch.size() - 1;
  const int maxCol = wordSearch[0].size() - 1;

  int occurences = 0;

  std::string xmas = "XMAS";

  std::string currentXmas = "X";
  int currentRow = row, currentCol = col;
  for (std::pair<int, int> direction : directions) {
    for (int i = 0; i < 3; i++) {
      currentCol += direction.first;  // x-value
      currentRow += direction.second; // y-value

      if (currentCol < 0 || currentCol > maxCol || currentRow < 0 ||
          currentRow > maxRow)
        break;

      currentXmas += wordSearch[currentRow][currentCol];
    }

    if (currentXmas == xmas)
      occurences++;

    currentXmas = "X";
    currentRow = row;
    currentCol = col;
  }

  return occurences;
}

bool isMasX(const std::vector<std::vector<char>> &wordSearch, int row,
            int col) {
  const int maxRow = wordSearch.size() - 1;
  const int maxCol = wordSearch[0].size() - 1;

  if (row - 1 < 0 || row + 1 > maxRow || col - 1 < 0 || col + 1 > maxCol)
    return false;

  std::string firstDiagonal = std::string(1, wordSearch[row - 1][col - 1]) +
                              "A" + wordSearch[row + 1][col + 1];
  std::string secondDiagonal = std::string(1, wordSearch[row - 1][col + 1]) +
                               "A" + wordSearch[row + 1][col - 1];

  bool firstIsMas = firstDiagonal == "SAM" || firstDiagonal == "MAS";
  bool secondIsMas = secondDiagonal == "SAM" || secondDiagonal == "MAS";

  return firstIsMas && secondIsMas;
}

int main() {
  std::string input = AOCHelper::readInput("Day4/input.txt");

  unsigned xmasOccurences = 0;
  unsigned masxoccurences = 0;

  const std::vector<std::vector<char>> parsedInput = splitInput(input);

  for (size_t row = 0; row < parsedInput.size(); row++)
    for (size_t col = 0; col < parsedInput[0].size(); col++) {
      if (parsedInput[row][col] == 'X')
        xmasOccurences += countXmasOccurences(parsedInput, row, col);
      if (parsedInput[row][col] == 'A')
        if (isMasX(parsedInput, row, col))
          masxoccurences++;
    }

  std::cout << "Part 1: " << xmasOccurences << std::endl;
  std::cout << "Part 2: " << masxoccurences << std::endl;

  return 0;
}
