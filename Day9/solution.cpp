#include "AOCHelper.h"

#include <iostream>
#include <string>
#include <vector>

typedef unsigned long long integer;

typedef struct s_DataBlock {
  int id;
  size_t size;
  size_t start;
} DataBlock;

void printDisk(const std::vector<DataBlock> &blocks) {
  for (const DataBlock &block : blocks) {
    for (size_t i = 0; i < block.size; i++)
      std::cout << static_cast<char>((block.id == -1) ? '.' : block.id + '0');
  }
  std::cout << '\n';
}

std::vector<int> parseInput(const std::string &input) {
  std::vector<int> result;

  for (char c : input)
    if (std::isdigit(c))
      result.push_back(c - '0');

  return result;
}

std::vector<std::string>
parseDiskMapFragmented(const std::vector<int> &diskMap) {
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

std::vector<DataBlock> parseDiskMap(const std::vector<int> &diskMap) {
  std::vector<DataBlock> disk;

  size_t spaceAllocated = 0;
  for (size_t i = 0; i < diskMap.size(); i++) {
    DataBlock block;
    block.size = diskMap[i];
    block.id = (i % 2 == 0) ? (i / 2) : (-1);
    block.start = spaceAllocated;
    spaceAllocated += block.size;
    disk.push_back(block);
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

std::vector<DataBlock> compactDisk(std::vector<DataBlock> disk) {
  int currentID;
  size_t upper = disk.size() - 1;

  while (disk[upper].id == -1)
    upper--;

  currentID = disk[upper].id;

  while (currentID > 0) {
    upper = disk.size() - 1;
    size_t lower = 0;

    // locate block
    while (disk[upper].id != currentID)
      upper--;

    // safely attempt to find space
    while (lower < disk.size() - 1 && disk[lower].start < disk[upper].start &&
           (disk[lower].size < disk[upper].size || disk[lower].id != -1))
      lower++;

    // not enough space, next id
    if (disk[lower].size < disk[upper].size || disk[lower].id != -1) {
      currentID--;
      continue;
    }

    // hawk tuah swap data blocks on that thang
    if (disk[lower].size == disk[upper].size) {
      disk[lower].id = disk[upper].id;
      disk[upper].id = -1;
    } else {
      size_t extraSpace = disk[lower].size - disk[upper].size;

      disk[lower].id = disk[upper].id;
      disk[upper].id = -1;
      disk[lower].size = disk[upper].size;

      DataBlock extraSpaceBlock;
      extraSpaceBlock.id = -1;
      extraSpaceBlock.size = extraSpace;
      extraSpaceBlock.start = disk[lower].start + disk[lower].size;

      disk.insert(disk.begin() + lower + 1, extraSpaceBlock);
    }
  }

  return disk;
}

integer calculateChecksumFragmented(const std::vector<std::string> &disk) {
  integer checkSum = 0;

  for (size_t i = 0; i < disk.size(); i++) {

    const std::string &diskBlock = disk[i];

    if (diskBlock == ".")
      continue;

    checkSum += i * std::stoull(diskBlock);
  }

  return checkSum;
}

integer calculateChecksum(const std::vector<DataBlock> &disk) {
  integer checkSum = 0;

  size_t trueIndex = 0;
  for (size_t i = 0; i < disk.size(); i++) {
    if (disk[i].id == -1) {
      trueIndex += disk[i].size;
      continue;
    }

    for (size_t j = 0; j < disk[i].size; j++)
      checkSum += trueIndex++ * disk[i].id;
  }

  return checkSum;
}

int main() {
  std::string input = AOCHelper::readInput("Day9/input.txt");

  std::vector<int> diskMap = parseInput(input);

  std::vector<std::string> disk = parseDiskMapFragmented(diskMap);

  std::vector<DataBlock> diskBlocks = parseDiskMap(diskMap);

  std::vector<std::string> compactedDiskFragmented =
      compactDiskFragmented(disk);

  std::vector<DataBlock> compactedDisk = compactDisk(diskBlocks);

  integer fragmentedCheckSum =
      calculateChecksumFragmented(compactedDiskFragmented);

  integer checkSum = calculateChecksum(compactedDisk);

  std::cout << "Part 1: " << fragmentedCheckSum << '\n';
  std::cout << "Part 2: " << checkSum << '\n';

  return 0;
}
