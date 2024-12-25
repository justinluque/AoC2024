#include "AOCHelper.h"

#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>

typedef unsigned long long bigint;

bigint totalDigits(bigint num) {
  if (num == 0)
    return 1;

  bigint digits = 0;

  do {
    digits++;
    num /= 10;
  } while (num > 0);

  return digits;
}

bool isEvenDigits(bigint num) { return totalDigits(num) % 2 == 0; }

std::pair<bigint, bigint> split(bigint num) {
  std::pair<bigint, bigint> result;
  std::string snum = std::to_string(num);

  bigint size = snum.size();

  result.first = std::stoull(snum.substr(0, size / 2));
  result.second = std::stoull(snum.substr(size / 2));

  return result;
}

std::map<bigint, bigint> parseInput(const std::string &input) {
  std::map<bigint, bigint> rocks;

  std::istringstream numTokenizer(input);
  std::string numToken;

  while (std::getline(numTokenizer, numToken, ' '))
    rocks[std::stoull(numToken)]++;

  return rocks;
}

void blink(std::map<bigint, bigint> &rocks) {
  std::map<bigint, bigint> newRocks;

  for (const auto &[stoneNum, count] : rocks) {
    if (stoneNum == 0)
      newRocks[1] += count;
    else if (isEvenDigits(stoneNum)) {
      auto splitNums = split(stoneNum);
      newRocks[splitNums.first] += count;
      newRocks[splitNums.second] += count;
    } else {
      newRocks[stoneNum * 2024] += count;
    }
  }

  rocks = std::move(newRocks);
}

bigint totalStonesAfterXBlinks(std::map<bigint, bigint> stones, size_t blinks) {
  for (size_t i = 0; i < blinks; i++)
    blink(stones);

  return std::accumulate(stones.begin(), stones.end(), 0,
                         [](bigint total, const std::pair<bigint, bigint> &p) {
                           return total + p.second;
                         });
}

int main() {
  std::string input = AOCHelper::readInput("Day11/sample_input.txt");

  std::map<bigint, bigint> stones = parseInput(input);

  bigint partOne = totalStonesAfterXBlinks(stones, 25);
  bigint partTwo = totalStonesAfterXBlinks(stones, 75);

  std::cout << "Part 1: " << partOne << '\n';
  std::cout << "Part 2: " << partTwo << '\n';

  for (int i = 1; i < 76; i++)
    std::cout << i << " - " << totalStonesAfterXBlinks(stones, i) << '\n';

  return 0;
}
