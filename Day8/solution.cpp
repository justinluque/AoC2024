#include "AOCHelper.h"

#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

std::vector<std::vector<char>> parseInput(const std::string &input) {
  std::vector<std::vector<char>> grid;

  std::istringstream lineTokenizer(input);
  std::string lineToken;

  while (std::getline(lineTokenizer, lineToken)) {
    std::vector<char> line;
    for (char c : lineToken) {
      line.push_back(c);
    }
    grid.push_back(line);
  }

  return grid;
}

std::unordered_map<char, std::set<std::pair<size_t, size_t>>>
locateAntennas(const std::vector<std::vector<char>> &grid) {
  std::unordered_map<char, std::set<std::pair<size_t, size_t>>> antennas;

  for (size_t row = 0; row < grid.size(); row++)
    for (size_t col = 0; col < grid.size(); col++)
      if (grid[row][col] != '.')
        antennas[grid[row][col]].insert(std::make_pair(row, col));

  for (const auto &[key, value] : antennas) {
    std::cout << "{" << key << ": ";
    for (auto pair : value)
      std::cout << "(Row: " << pair.first << ", Col: " << pair.second << "), ";
    std::cout << '\n';
  }
  return antennas;
}

bool fitsInGrid(std::pair<size_t, size_t> coords,
                const std::vector<std::vector<char>> &grid) {
  size_t maxY = grid.size() - 1;
  size_t maxX = grid[0].size() - 1;

  return !(coords.first > maxY || coords.second > maxX);
}

std::set<std::pair<size_t, size_t>>
locateAntinodes(const std::vector<std::vector<char>> &grid) {
  auto antennas = locateAntennas(grid);

  std::set<std::pair<size_t, size_t>> antinodes;

  for (const auto &[antenna, locations] : antennas) {
    for (auto location1 : locations) {
      for (auto location2 : locations) {
        if (location1 == location2)
          continue;

        int dx = location2.second - location1.second;
        int dy = location2.first - location1.first;

        size_t midX = (location1.second + location2.second) / 2;
        size_t midY = (location1.first + location2.first) / 2;

        auto antinode1 = std::make_pair(midY - dy, midX - dx);
        auto antinode2 = std::make_pair(midY + dy, midX + dx);

        if (fitsInGrid(antinode1, grid))
          antinodes.insert(antinode1);
        else
          std::cout << "Antinode at row" << antinode1.second << " and col "
                    << antinode2.second << "doesnt fit\n";
        if (fitsInGrid(antinode2, grid))
          antinodes.insert(antinode2);
        else
          std::cout << "Antinode at row" << antinode1.second << " and col "
                    << antinode2.second << "doesnt fit\n";
      }
    }
  }

  return antinodes;
}

unsigned countAntinodes(const std::vector<std::vector<char>> &grid) {
  auto antinodes = locateAntinodes(grid);

  return antinodes.size();
}

int main() {
  std::string input = AOCHelper::readInput("Day8/input.txt");

  auto grid = parseInput(input);

  unsigned antinodes = countAntinodes(grid);

  std::cout << "Part 1: " << antinodes << '\n';

  return 0;
}
