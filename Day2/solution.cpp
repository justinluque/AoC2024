#include "AOCHelper.h"

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>

enum class Direction
{
  Stagnant,
  Increasing,
  Decreasing
};

Direction getDirection(int x1, int x2)
{
  if (x1 == x2)
    return Direction::Stagnant;
  else if (x2 > x1)
    return Direction::Increasing;
  else // x2 < x1
    return Direction::Decreasing;
}

int getDistance(int x1, int x2)
{
  return std::abs(x2 - x1);
}

/**
 * Two requirements for levels to be safe:
 * - Levels must have unchanging direction (cannot be stagnant)
 * - Any two adjacent levels must differ by at least one
 *  but at most three
 *  */
bool isSafeLevels(const std::vector<int> &levels)
{
  int totalLevels = levels.size();

  if (levels.size() == 1)
    return true;

  Direction initialDirection = getDirection(levels[0], levels[1]);

  if (initialDirection == Direction::Stagnant)
  {
    return false;
  }

  for (int index = 0; index < totalLevels - 1; index++)
  {
    if (getDirection(levels[index], levels[index + 1]) != initialDirection)
      return false;

    int distance = getDistance(levels[index], levels[index + 1]);

    if (distance < 1 || distance > 3)
      return false;
  }

  return true;
}

int main()
{
  std::string input = AOCHelper::readInput("Day2/input.txt");

  std::istringstream fileTokenizer(input);

  std::string fileToken;

  unsigned totalSafeReports = 0;

  while (std::getline(fileTokenizer, fileToken))
  {
    std::vector<int> report;

    std::istringstream tokenizer(fileToken);

    std::string token;

    while (std::getline(tokenizer, token, ' '))
    {
      report.push_back(std::stoi(token));
    }

    if (isSafeLevels(report))
      totalSafeReports++;
  }

  std::cout << totalSafeReports << std::endl;

  return 0;
}
