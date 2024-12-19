#include "AOCHelper.h"

#include <iostream>
#include <string>
#include <vector>

typedef unsigned long long integer;

std::vector<int> parseInput(const std::string &input) {
  std::vector<int> result;

  for (char c : input)
    if (std::isdigit(c))
      result.push_back(c - '0');

  return result;
}

std::vector<std::string> parseDiskMap(const std::vector<int> &diskMap) {
  std::vector<std::string> disk;

  for (size_t i = 0; i < diskMap.size(); i++) {
    std::string insertString;
    if (i % 2 == 0) {
      insertString = std::to_string(i / 2);
    } else {
      insertString = ".";
    }
    disk.insert(disk.end(), diskMap[i], insertString);
  }

  return disk;
}

std::vector<std::string> compactDiskFragmented(std::vector<std::string> disk) {
  size_t left = 0;
  size_t right = disk.size() - 1;

  while (left < right) {
    while (disk[left] != ".")
      left++;

    if (left >= right)
      break;

    disk[left] = disk[right];
    disk[right] = ".";
    right--;
  }

  return disk;
}

integer calculateChecksum(const std::vector<std::string> &disk) {
  integer checkSum = 0;

  for (size_t i = 0; i < disk.size(); i++) {

    const std::string &diskBlock = disk[i];

    if (diskBlock == ".")
      continue;

    checkSum += i * std::stoull(diskBlock);
  }

  return checkSum;
}

int main() {
  std::string input = AOCHelper::readInput("Day9/input.txt");

  std::vector<int> diskMap = parseInput(input);

  std::vector<std::string> disk = parseDiskMap(diskMap);

  std::vector<std::string> compactedDiskFragmented =
      compactDiskFragmented(disk);

  integer fragmentedCheckSum = calculateChecksum(compactedDiskFragmented);

  std::cout << "Part 1: " << fragmentedCheckSum << '\n';

  return 0;
}
