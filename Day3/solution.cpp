#include "AOCHelper.h"

#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <regex>

int main()
{
  std::string input = AOCHelper::readInput("Day3/input.txt");

  std::regex pattern(R"(mul\((\d+),(\d+)\))");
  std::smatch match;
  int sumOfMultiplications = 0;

  auto begin = input.cbegin();
  auto end = input.cend();

  while (std::regex_search(begin, end, match, pattern))
  {
    int firstNum = std::stoi(match[1].str());
    int secondNum = std::stoi(match[2].str());

    sumOfMultiplications += firstNum * secondNum;
    begin = match.suffix().first;
  }

  std::cout << sumOfMultiplications << std::endl;
}
