#include "AOCHelper.h"

#include <array>
#include <iostream>
#include <set>
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

bool findLoop(std::vector<std::vector<char>> grid, size_t startingRow,
              size_t startingCol, Direction startingDirection) {
  size_t maxRow = grid.size();
  size_t maxCol = grid[0].size();

  std::vector<std::vector<int>> timesVisited(maxRow,
                                             std::vector<int>(maxCol, 0));

  size_t guardRow = startingRow;
  size_t guardCol = startingCol;
  Direction currentDirection = startingDirection;

  while (!isOutOfBounds(guardRow, guardCol, maxRow, maxCol, currentDirection)) {
    std::pair<int, int> directionVector = toVector(currentDirection);

    size_t newPosRow = guardRow + directionVector.first;
    size_t newPosCol = guardCol + directionVector.second;

    if (isObstruction(grid[newPosRow][newPosCol])) {
      currentDirection = rotateClockwise(currentDirection);
      continue;
    }

    if (timesVisited[guardRow][guardCol] >= 4)
      return true;

    grid[newPosRow][newPosCol] = directionToGuard(currentDirection);
    grid[guardRow][guardCol] = 'X';
    timesVisited[guardRow][guardCol]++;

    guardRow = newPosRow;
    guardCol = newPosCol;
  }

  return false;
}

unsigned countLoopCausingNewObstructions(std::vector<std::vector<char>> grid) {
  unsigned loopsFound = 0;

  size_t totalRows = grid.size();
  size_t totalCols = grid[0].size();

  size_t startingGuardRow, startingGuardCol;

  for (size_t i = 0; i < totalRows; i++)
    for (size_t j = 0; j < totalCols; j++)
      if (isGuard(grid[i][j])) {
        startingGuardRow = i;
        startingGuardCol = j;
      }

  Direction startingDirection =
      guardToDirection(grid[startingGuardRow][startingGuardCol]);

  std::vector<std::vector<char>> gridCopy = grid;
  size_t guardPosRow = startingGuardRow;
  size_t guardPosCol = startingGuardCol;
  Direction currentDirection = startingDirection;

  std::set<std::pair<size_t, size_t>> locationsToTest;

  // Traverse grid looking for locations to test
  while (!isOutOfBounds(guardPosRow, guardPosCol, totalRows, totalCols,
                        currentDirection)) {
    std::pair<int, int> directionVector = toVector(currentDirection);

    size_t newPosRow = guardPosRow + directionVector.first;
    size_t newPosCol = guardPosCol + directionVector.second;

    if (isObstruction(gridCopy[newPosRow][newPosCol])) {
      currentDirection = rotateClockwise(currentDirection);
      continue;
    }

    if (!locationsToTest.contains(std::make_pair(newPosRow, newPosCol)))
      locationsToTest.insert(
          std::make_pair(newPosRow, newPosCol)); // append location to set

    gridCopy[newPosRow][newPosCol] = directionToGuard(currentDirection);
    gridCopy[guardPosRow][guardPosCol] = 'X';

    guardPosRow = newPosRow;
    guardPosCol = newPosCol;
  }

  for (const std::pair<size_t, size_t> &location : locationsToTest) {
    grid[location.first][location.second] = '#';
    if (findLoop(grid, startingGuardRow, startingGuardCol, startingDirection))
      loopsFound++;
    grid[location.first][location.second] = '.';
  }

  return loopsFound;
}

int main() {
  std::string input = AOCHelper::readInput("Day6/input.txt");

  std::vector<std::vector<char>> grid = parseInput(input);

  unsigned uniquePositions = countUniquePositionsReached(grid);
  unsigned loopsFound = countLoopCausingNewObstructions(grid);

  std::cout << "Part 1: " << uniquePositions << '\n';
  std::cout << "Part 2: " << loopsFound << '\n';

  return 0;
}
