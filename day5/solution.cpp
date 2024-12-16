#include "AOCHelper.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>
#include <exception>

std::unordered_map<int, std::vector<int>> extractRules(const std::string &input)
{
	std::unordered_map<int, std::vector<int>> rules;

	std::istringstream lineTokenizer(input);
	std::string lineToken;

	while(std::getline(lineTokenizer, lineToken))
	{
		if (lineToken == std::string(""))
			break;

		int delimiterIndex = lineToken.find("|");
		int beforeNumber = std::stoi(lineToken.substr(0, delimiterIndex));
		int afterNumber = std::stoi(lineToken.substr(delimiterIndex + 1, lineToken.size() - 1));

    if(rules.contains(afterNumber))
      rules[afterNumber].push_back(beforeNumber);
    else
      rules[afterNumber] = {beforeNumber};
	}

	return rules;
}

std::vector<std::vector<int>> extractUpdates(const std::string &input)
{
	std::vector<std::vector<int>> updates;

	std::string inputUpdates = input.substr(input.find("\n\n") + 2, input.size() - 1);

	std::istringstream lineTokenizer(inputUpdates);
	std::string lineToken;

	while(std::getline(lineTokenizer, lineToken))
	{
		std::vector<int> update;

		std::istringstream numTokenizer(lineToken);
		std::string numToken;

		while (std::getline(numTokenizer, numToken, ','))
		{
			int num = std::stoi(numToken);
			update.push_back(num);
		}

		updates.push_back(update);
	}

	return updates;
}

bool isCorrectOrder(const std::vector<int> &update, const std::unordered_map<int, std::vector<int>> &rules)
{
	for(size_t i = 0; i < update.size(); i++)
	{
		if (rules.contains(update[i]))
		{
      for (int num : rules.at(update[i]))
      {
        if(num == update[i])
          continue;

        auto potentialRuleViolation = std::find(update.begin() + i + 1, update.end(), num);
			  bool foundViolation = potentialRuleViolation != update.end();
			  if (foundViolation)
				  return false;
      }
		}
	}

	return true;
}

int getMiddlePageNumber(const std::vector<int> &update)
{
	if (update.size() % 2 == 0)
		throw std::domain_error("Cannot find medium page: number of updates is non-even"); // I assume'd we got an odd number of pages in each report
	
	return update[update.size() / 2];
}

int sumMiddlePagesOfCorrectReports(const std::vector<std::vector<int>> &updates, const std::unordered_map<int, std::vector<int>> &rules)
{
	int sum = 0;

	for(const std::vector<int> &update : updates)
		if(isCorrectOrder(update, rules))
			sum += getMiddlePageNumber(update);

	return sum;
}

int main()
{
	std::string input = AOCHelper::readInput("Day5/input.txt");
	std::unordered_map<int, std::vector<int>> rules = extractRules(input);
	std::vector<std::vector<int>> updates = extractUpdates(input);	

	int sum = sumMiddlePagesOfCorrectReports(updates, rules);

	std::cout << "Part 1: " << sum << '\n';

	return 0;
}
