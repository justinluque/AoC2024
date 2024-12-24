#include "AOCHelper.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <array>
#include <utility>
#include <cmath>

constexpr std::array<std::pair<int, int>, 4> directions = {{
	{1, 0},
	{-1, 0},
	{0, 1},
	{0, -1}
}};

bool transformationFits(size_t row, size_t col, const std::pair<int, int> &transform, const std::vector<std::vector<int>> &arr)
{
	bool underflow = (transform.first < 0 && std::abs(transform.first) > row) || (transform.second < 0 && std::abs(transform.second) > col);
	bool overflow = (transform.first > 0 && row + transform.first >= arr.size()) || (transform.second > 0 && col + transform.second >= arr[0].size());
	return !(underflow || overflow);
}

std::vector<std::vector<int>> parseInput(const std::string &input)
{
	std::vector<std::vector<int>> result;

	std::istringstream lineTokenizer(input);
	std::string lineToken;

	while(std::getline(lineTokenizer, lineToken))
	{
		std::vector<int> line;
		for (char c : lineToken)
			if(std::isdigit(c))
				line.push_back(c - '0');
		result.push_back(line);
	}

	return result;
}

unsigned countPaths(const std::vector<std::vector<int>> &map, std::vector<std::vector<bool>> &visited, size_t row, size_t col)
{
	unsigned occurences = 0;
	const int currentElevation = map[row][col];
	visited[row][col] = true;

	for (const auto &direction : directions)
	{
		if(!transformationFits(row, col, direction, map))
			continue;

		const size_t newRow = row + direction.first;
		const size_t newCol = col + direction.second;
		const int newElevation = map[newRow][newCol];

		if (newElevation == 9 && !visited[newRow][newCol])
		{	occurences++; 
			visited[newRow][newCol] = true;
		} else if (newElevation == currentElevation + 1 && !visited[newRow][newCol])
			occurences += countPaths(map, visited, newRow, newCol);
	}
	return occurences;
}
			


unsigned sumTrailheadScores(const std::vector<std::vector<int>> &map)
{
	unsigned sum = 0;
	for (size_t row = 0; row < map.size(); row++)
		for (size_t col = 0; col < map[0].size(); col++)
			if (map[row][col] == 0)
			{
				std::vector<std::vector<bool>> visited(map.size(), std::vector<bool>(map[0].size(), false));
				sum += countPaths(map, visited, row, col);
			}
	return sum;
}

int main()
{
	std::string input = AOCHelper::readInput("Day10/input.txt");

	std::vector<std::vector<int>> map = parseInput(input);

	const unsigned sumScores = sumTrailheadScores(map);

	std::cout << "Part 1: " << sumScores << '\n';
}
