#include "AOCHelper.h"

#include <cassert>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <cmath>

std::string trim(const std::string &str)
{
  std::string result = str;

  // Trim leading whitespace
  result.erase(result.begin(), std::find_if_not(result.begin(), result.end(), ::isspace));

  // Trim trailing whitespace
  result.erase(std::find_if_not(result.rbegin(), result.rend(), ::isspace).base(), result.end());

  return result;
}

int main()
{
  const std::string input = trim(AOCHelper::readInput("Day1/input.txt"));

  std::vector<int> leftNums, rightNums;

  std::istringstream stringIterator(input);

  std::string tempToken;

  while (std::getline(stringIterator, tempToken))
  {
    int delimiterIndex = tempToken.find(" ");
    leftNums.push_back(std::stoi(tempToken.substr(0, delimiterIndex)));
    rightNums.push_back(std::stoi(tempToken.substr(delimiterIndex + 1, tempToken.size() - 1)));
  }

  std::sort(leftNums.begin(), leftNums.end());

  std::sort(rightNums.begin(), rightNums.end());

  int totalNums = leftNums.size();

  int totalDistance = 0;

  for (int index = 0; index < totalNums; index++)
  {
    totalDistance += std::abs(rightNums[index] - leftNums[index]);
  }

  std::cout << totalDistance << '\n';
  return 0;
}
