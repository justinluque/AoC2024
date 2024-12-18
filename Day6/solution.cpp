#include "AOCHelper.h"

#include <array>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

enum class Direction { UP, RIGHT, DOWN, LEFT };

constexpr std::array<std::pair<int, int>, 4> directions = {{
    {-1, 0}, // UP
    {0, 1},  // RIGHT
    {1, 0},  // DOWN
    {0, -1}  // LEFT
}};

constexpr std::pair<int, int> toVector(Direction direction) {
  return directions[static_cast<size_t>(direction)];
}

Direction rotateClockwise(Direction direction) {
  size_t u_direction = static_cast<size_t>(direction);

  u_direction = (u_direction + 1) % 4;

  return static_cast<Direction>(u_direction);
}

std::vector<std::vector<char>> parseInput(const std::string &input) {
  std::vector<std::vector<char>> lines;

  std::istringstream lineTokenizer(input);
  std::string lineToken;

  while (std::getline(lineTokenizer, lineToken)) {
    std::vector<char> line;

    for (const char &cell : lineToken)
      line.push_back(cell);

    lines.push_back(line);
  }

  return lines;
}

bool isGuard(char c) { return c == '^' || c == '>' || c == '<' || c == 'v'; }

bool isObstruction(char c) { return c == '#'; }

bool isUnvisited(char c) { return c == '.'; }

Direction guardToDirection(char c) {
  switch (c) {
  case '^':
    return Direction::UP;
  case '>':
    return Direction::RIGHT;
  case 'v':
    return Direction::DOWN;
  case '<':
    return Direction::LEFT;
  default:
    throw std::domain_error(
        std::string("Cannot interpret direction from guard character: ") + c);
  }
}

char directionToGuard(Direction direction) {
  switch (direction) {
  case Direction::UP:
    return '^';
  case Direction::RIGHT:
    return '>';
  case Direction::DOWN:
    return 'v';
  case Direction::LEFT:
    return '<';
  default:
    throw std::domain_error("???");
  }
}

bool isOutOfBounds(size_t currentRow, size_t currentCol, size_t maxRow,
                   size_t maxCol, Direction direction) {
  switch (direction) {
  case Direction::UP:
    return currentRow == 0;
  case Direction::DOWN:
    return currentRow + 1 >= maxRow;
  case Direction::LEFT:
    return currentCol == 0;
  case Direction::RIGHT:
    return currentCol + 1 >= maxCol;
  default:
    throw std::domain_error("???");
  }
}

unsigned countUniquePositionsReached(std::vector<std::vector<char>> grid) {
  unsigned uniquePositions = 1;

  size_t totalRows = grid.size();
  size_t totalCols = grid[0].size();

  size_t guardPosRow, guardPosCol;
  Direction currentDirection;

  for (size_t i = 0; i < totalRows; i++)
    for (size_t j = 0; j < totalCols; j++)
      if (isGuard(grid[i][j])) {
        guardPosRow = i;
        guardPosCol = j;
      }

  currentDirection = guardToDirection(grid[guardPosRow][guardPosCol]);

  while (!isOutOfBounds(guardPosRow, guardPosCol, totalRows, totalCols,
                        currentDirection)) {
    std::pair<int, int> directionVector = toVector(currentDirection);

    size_t newPosRow = guardPosRow + directionVector.first;
    size_t newPosCol = guardPosCol + directionVector.second;

    if (isObstruction(grid[newPosRow][newPosCol])) {
      currentDirection = rotateClockwise(currentDirection);
      continue;
    }

    if (isUnvisited(grid[newPosRow][newPosCol]))
      uniquePositions++;

    grid[newPosRow][newPosCol] = directionToGuard(currentDirection);
    grid[guardPosRow][guardPosCol] = 'X';

    guardPosRow = newPosRow;
    guardPosCol = newPosCol;
  }

  return uniquePositions;
}

int main() {
  std::string input = AOCHelper::readInput("Day6/input.txt");

  std::vector<std::vector<char>> grid = parseInput(input);

  unsigned uniquePositions = countUniquePositionsReached(grid);

  std::cout << "Part 1: " << uniquePositions << '\n';

  return 0;
}
