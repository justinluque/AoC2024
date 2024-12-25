#include "AOCHelper.h"

#include <iostream>
#include <vector>
#include <sstream>
#include <string>

typedef unsigned long long bigint;

void printVec(const std::vector<bigint> &vec)
{
	for(const bigint &num : vec)
		std::cout << num << ' ';
	std::cout << '\n';
}

unsigned totalDigits(bigint num)
{
	if (num == 0)
		return 1;

	unsigned digits = 0;

	do
	{
		digits++;
		num /= 10;
	} while (num > 0);

	return digits;
}

bool isEvenDigits(bigint num)
{
	return totalDigits(num) % 2 == 0;
}

std::pair<bigint, bigint> split(bigint num)
{
	std::pair<bigint, bigint> result;
	std::string snum = std::to_string(num);
	size_t size = snum.size();
	result.first = std::stoull(snum.substr(0, size / 2));
	result.second = std::stoull(snum.substr(size / 2));
	return result;
}

std::vector<bigint> parseInput(const std::string &input)
{
	std::vector<bigint> rocks;

	std::istringstream numTokenizer(input);
	std::string numToken;

	while(std::getline(numTokenizer, numToken, ' '))
		rocks.push_back(std::stoi(numToken));

	return rocks;
}

void blink(std::vector<bigint> &rocks)
{
	for (size_t i = 0; i < rocks.size(); i++)
	{
		bigint stoneNum = rocks[i];
		if (stoneNum == 0)
			rocks[i] = 1;
		else if (isEvenDigits(stoneNum))
		{
			auto splitNums = split(rocks[i]);
			rocks.insert(rocks.begin() + i, splitNums.first);
			rocks[++i] = splitNums.second;
		} else rocks[i] *= 2024;
	}
}

size_t totalStonesAfterXBlinks(std::vector<bigint> stones, size_t blinks)
{
	for (size_t i = 0; i < blinks; i++)
	{
		printVec(stones);
		std::cout << "Calculating blink " << i + 1;
		blink(stones);
	}

	printVec(stones);
	return stones.size();
}

int main()
{
	std::string input = AOCHelper::readInput("Day11/input.txt");

	std::vector<bigint> stones = parseInput(input);

	size_t partOne = totalStonesAfterXBlinks(stones, 25);

	std::cout << "Part 1: " << partOne << '\n';

	return 0;
}

