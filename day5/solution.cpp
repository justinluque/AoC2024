#include "AOCHelper.h"

#include <algorithm>
#include <iostream>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

void printUpdate(const std::vector<int> &update) {
  for (int num : update)
    std::cout << num << ' ';
  std::cout << '\n';
}

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
  const std::unordered_map<int, std::vector<int>> adj = parseRules(rules);

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

// xth indegree: number of jobs that must occur before the xth job
std::unordered_map<int, int>
calculateInDegrees(const std::vector<int> &update,
                   const std::unordered_map<int, std::vector<int>> &rules) {
  std::unordered_map<int, int> inDegree;

  for (int job : update)
    inDegree[job] = 0;

  for (const auto &[job, dependencies] : rules) {
    for (int dependentJob : dependencies) {
      if (inDegree.contains(dependentJob) && inDegree.contains(job))
        inDegree[dependentJob]++;
    }
  }

  return inDegree;
}

std::vector<int>
topologicalSort(const std::vector<int> &update,
                const std::vector<std::pair<int, int>> &rules) {
  const std::unordered_map<int, std::vector<int>> adj = parseRules(rules);
  std::unordered_map<int, int> inDegree = calculateInDegrees(update, adj);
  std::queue<int> jobQueue;
  std::vector<int> sorted;

  for (const auto &[key, value] : inDegree)
    std::cout << key << " is depended on by " << value << " jobs.\n";

  for (int num : update) {
    if (inDegree.at(num) == 0)
      jobQueue.push(num);
  }

  while (!jobQueue.empty()) {
    int job = jobQueue.front();
    jobQueue.pop();
    sorted.push_back(job);

    if (adj.contains(job)) {
      for (int neighbor : adj.at(job)) {
        if (inDegree.contains(neighbor)) {
          inDegree[neighbor]--;

          if (inDegree[neighbor] == 0)
            jobQueue.push(neighbor);
        }
      }
    }
  }

  std::cout << "Unsorted: ";
  printUpdate(update);
  std::cout << "Sorted: ";
  printUpdate(sorted);

  if (sorted.size() != update.size()) {
    throw std::domain_error("Could not topologically sort, cycle detected");
  }

  return sorted;
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

int sumMiddlePagesOfSortedReports(
    const std::vector<std::vector<int>> &updates,
    const std::vector<std::pair<int, int>> &rules) {

  int sum = 0;
  for (const std::vector<int> &update : updates) {
    if (!isCorrectOrder(update, rules)) {
      std::vector<int> sorted = topologicalSort(update, rules);
      sum += getMiddlePageNumber(sorted);
    }
  }

  return sum;
}

int main() {
  std::string input = AOCHelper::readInput("Day5/input.txt");
  std::vector<std::pair<int, int>> rules = extractRules(input);
  std::vector<std::vector<int>> updates = extractUpdates(input);

  int sumOfCorrectMiddlePages = sumMiddlePagesOfCorrectReports(updates, rules);
  int sumOfsortedMiddlePages = sumMiddlePagesOfSortedReports(updates, rules);

  std::cout << "Part 1: " << sumOfCorrectMiddlePages << '\n';
  std::cout << "Part 2:" << sumOfsortedMiddlePages << '\n';

  return 0;
}
