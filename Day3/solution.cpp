#include "AOCHelper.h"

#include <iostream>
#include <regex>
#include <string>

int computeSumOfMultiplication(const std::string &data) {
  const std::regex pattern(R"(mul\((\d+),(\d+)\))");
  std::smatch matches;
  auto searchBegin = data.cbegin();
  int sumOfMultiplications = 0;

  while (std::regex_search(searchBegin, data.cend(), matches, pattern)) {
    int firstNum = std::stoi(matches[1].str());
    int secondNum = std::stoi(matches[2].str());

    sumOfMultiplications += firstNum * secondNum;
    searchBegin = matches.suffix().first;
  }

  return sumOfMultiplications;
}

int computeConditionalSumOfMultiplication(const std::string &data) {
  const std::regex pattern(R"((don't\(\))|(do\(\))|mul\((\d+),(\d+)\))");

  std::smatch matches;
  auto searchBegin = data.cbegin();
  int sumOfMultiplications = 0;
  bool calculate = true;

  while (std::regex_search(searchBegin, data.cend(), matches, pattern)) {
    if (matches[1].matched) // Match for "don't()"
    {
      calculate = false;
    } else if (matches[2].matched) // Match for "do()"
    {
      calculate = true;
    } else if (calculate && matches[3].matched &&
               matches[4].matched) // Match for "mul(x,y)"
    {
      int firstNum = std::stoi(matches[3].str());
      int secondNum = std::stoi(matches[4].str());
      sumOfMultiplications += firstNum * secondNum;
    }

    searchBegin = matches.suffix().first;
  }

  return sumOfMultiplications;
}

int main() {
  const std::string input = AOCHelper::readInput("Day3/input.txt");

  const int sumOfMultiplications = computeSumOfMultiplication(input);
  const int sumOfMultiplicationsProcessed =
      computeConditionalSumOfMultiplication(input);

  std::cout << "Part 1: " << sumOfMultiplications << '\n';
  std::cout << "Part 2: " << sumOfMultiplicationsProcessed << '\n';

  return 0;
}
