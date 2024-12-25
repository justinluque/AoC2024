#include "AOCHelper.h"

#include <array>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

constexpr std::array<std::pair<int, int>, 4> directions = {{
    {0, 1},
    {0, -1},
    {1, 0},
    {-1, 0},
}};

template <typename T>
bool transformationFits(size_t row, size_t col,
                        const std::pair<int, int> &transform,
                        const std::vector<std::vector<T>> &arr) {
  bool underflow = (transform.first < 0 &&
                    static_cast<size_t>(std::abs(transform.first)) > row) ||
                   (transform.second < 0 &&
                    static_cast<size_t>(std::abs(transform.second)) > col);
  bool overflow =
      (transform.first > 0 && row + transform.first >= arr.size()) ||
      (transform.second > 0 && col + transform.second >= arr[0].size());
  return !(underflow || overflow);
}

std::vector<std::vector<char>> parseInput(const std::string &input) {
  std::vector<std::vector<char>> grid;

  std::istringstream lineTokenizer(input);
  std::string lineToken;

  while (std::getline(lineTokenizer, lineToken)) {
    std::vector<char> line;

    for (char c : lineToken)
      line.push_back(c);

    grid.push_back(line);
  }

  return grid;
}

/// Returns <area, perimeter>
std::pair<int, int> fillRegion(const std::vector<std::vector<char>> &farmGrid,
                               std::vector<std::vector<bool>> &visited,
                               size_t row, size_t col) {
  int perimeter = 0;
  int area = 0;
  char target = farmGrid[row][col];

  std::queue<std::pair<int, int>> toVisit;
  toVisit.emplace(row, col);
  visited[row][col] = true;

  while (!toVisit.empty()) {
    auto currentCell = toVisit.front();
    toVisit.pop();
    area++;

    for (const auto &direction : directions) {
      if (!transformationFits(currentCell.first, currentCell.second, direction,
                              farmGrid)) {
        perimeter++;
        continue;
      }

      auto consideredCell =
          std::make_pair(currentCell.first + direction.first,
                         currentCell.second + direction.second);
      char cellsValue = farmGrid[consideredCell.first][consideredCell.second];

      if (target != cellsValue)
        perimeter++;
      else if (!visited[consideredCell.first][consideredCell.second]) {
        toVisit.push(consideredCell);
        visited[consideredCell.first][consideredCell.second] =
            true; // avoid multiple visits of the same cell
      }
    }
  }

  return std::make_pair(area, perimeter);
}

int sumAllRegionPrices(const std::vector<std::vector<char>> &farmGrid) {
  int sum = 0;
  std::vector<std::vector<bool>> visited(
      farmGrid.size(), std::vector<bool>(farmGrid[0].size(), false));

  for (size_t row = 0; row < farmGrid.size(); row++) {
    for (size_t col = 0; col < farmGrid[0].size(); col++) {
      if (!visited[row][col]) {
        auto plotData =
            fillRegion(farmGrid, visited, row, col); // <area, perimter>
        sum += plotData.first * plotData.second;
        std::cout << farmGrid[row][col] << " plot - " << plotData.first
                  << " area and " << plotData.second << " perimeter.\n";
      }
    }
  }

  return sum;
}

int main() {
  std::string input = AOCHelper::readInput("Day12/input.txt");

  std::vector<std::vector<char>> farmGrid = parseInput(input);

  int sumPrices = sumAllRegionPrices(farmGrid);

  std::cout << "Part 1: " << sumPrices << '\n';

  return 0;
}
