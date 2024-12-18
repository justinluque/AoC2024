#include "AOCHelper.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

std::vector<std::pair<int, int>> extractRules(const std::string &input) {
  std::vector<std::pair<int, int>> rules;

  std::istringstream lineTokenizer(input);
  std::string lineToken;

  while (std::getline(lineTokenizer, lineToken)) {
    if (lineToken == std::string(""))
      break;

    int delimiterIndex = lineToken.find("|");
    int beforeNumber = std::stoi(lineToken.substr(0, delimiterIndex));
    int afterNumber =
        std::stoi(lineToken.substr(delimiterIndex + 1, lineToken.size() - 1));

    rules.push_back(std::make_pair(beforeNumber, afterNumber));
  }

  return rules;
}

std::vector<std::vector<int>> extractUpdates(const std::string &input) {
  std::vector<std::vector<int>> updates;

  std::string inputUpdates =
      input.substr(input.find("\n\n") + 2, input.size() - 1);

  std::istringstream lineTokenizer(inputUpdates);
  std::string lineToken;

  while (std::getline(lineTokenizer, lineToken)) {
    std::vector<int> update;

    std::istringstream numTokenizer(lineToken);
    std::string numToken;

    while (std::getline(numTokenizer, numToken, ',')) {
      int num = std::stoi(numToken);
      update.push_back(num);
    }

    updates.push_back(update);
  }

  return updates;
}

// map of (first, list of last)
std::unordered_map<int, std::vector<int>>
parseRules(const std::vector<std::pair<int, int>> &rules) {
  std::unordered_map<int, std::vector<int>> adj;

  for (const std::pair<int, int> &rule : rules) {
    adj[rule.first].push_back(rule.second);
  }

  return adj;
}

bool isCorrectOrder(const std::vector<int> &update,
                    const std::vector<std::pair<int, int>> &rules) {
  std::unordered_map<int, std::vector<int>> adj = parseRules(rules);

  for (size_t i = 0; i < update.size(); i++) {
    int job = update[i];
    if (adj.contains(job)) {
      for (int after : adj.at(job)) {
        auto violationSearch =
            std::find(update.cbegin(), update.cbegin() + i, after);
        bool violationFound = violationSearch != update.cbegin() + i;

        if (violationFound)
          return false;
      }
    }
  }

  return true;
}

int getMiddlePageNumber(const std::vector<int> &update) {
  if (update.size() % 2 == 0)
    throw std::domain_error("Cannot find medium page: number of updates is "
                            "non-even"); // I assume'd we got an odd number of
                                         // pages in each report

  return update[update.size() / 2];
}

int sumMiddlePagesOfCorrectReports(
    const std::vector<std::vector<int>> &updates,
    const std::vector<std::pair<int, int>> &rules) {
  int sum = 0;

  for (const std::vector<int> &update : updates)
    if (isCorrectOrder(update, rules))
      sum += getMiddlePageNumber(update);

  return sum;
}

int main() {
  std::string input = AOCHelper::readInput("Day5/input.txt");
  std::vector<std::pair<int, int>> rules = extractRules(input);
  std::vector<std::vector<int>> updates = extractUpdates(input);

  int sum = sumMiddlePagesOfCorrectReports(updates, rules);

  std::cout << "Part 1: " << sum << '\n';

  return 0;
}
