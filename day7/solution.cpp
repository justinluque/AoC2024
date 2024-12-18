#include "AOCHelper.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

typedef struct s_equation {
  unsigned long long solution;
  std::vector<unsigned long long> terms;
} Equation;

void printEquation(const Equation &equation) {
  std::cout << equation.solution << ": ";

  for (unsigned long long num : equation.terms)
    std::cout << num << ' ';

  std ::cout << '\n';
}

std::vector<Equation> parseInput(const std::string &input) {
  std::vector<Equation> equations;

  std::istringstream lineTokenizer(input);
  std::string lineToken;

  while (std::getline(lineTokenizer, lineToken)) {
    Equation equation;

    size_t delimIndex = lineToken.find(": ");

    std::cout << lineToken.substr(0, delimIndex) << '\n';
    equation.solution = std::stoull(lineToken.substr(0, delimIndex));

    std::istringstream termTokenizer(
        lineToken.substr(delimIndex + 2, lineToken.size() - 1));
    std::string termToken;

    while (std::getline(termTokenizer, termToken, ' ')) {
      std::cout << termToken << '\n';
      equation.terms.push_back(std::stoull(termToken));
    }

    equations.push_back(equation);
  }

  return equations;
}

unsigned long long evaluateExpression(const std::vector<unsigned long long> &expression,
                                 const std::vector<char> &operators) {
  unsigned long long result = expression[0];

  for (size_t i = 1; i < expression.size(); ++i) {
    if (operators[i - 1] == '+') {
      result += expression[i];
    } else if (operators[i - 1] == '*') {
      result *= expression[i];
    }
  }

  return result;
}

bool checkSolution(const Equation &equation, std::vector<char> &operators,
                   size_t index) {
  if (index == equation.terms.size() - 1) {
    unsigned long long result = evaluateExpression(equation.terms, operators);
    return result == equation.solution;
  }

  operators[index] = '+';
  if (checkSolution(equation, operators, index + 1))
    return true;

  operators[index] = '*';
  return checkSolution(equation, operators, index + 1);
}

bool isValidEquation(const Equation &equation) {
  std::vector<char> operators(equation.terms.size() - 1);
  return checkSolution(equation, operators, 0);
}

unsigned long long sumValidEquations(const std::vector<Equation> &equations) {
  unsigned long long sum = 0;

  for (const Equation &equation : equations) {
    if (isValidEquation(equation))
      sum += equation.solution;
  }

  return sum;
}

int main() {
  const std::string input = AOCHelper::readInput("Day7/input.txt");

  const std::vector<Equation> equations = parseInput(input);

  const unsigned long long sumOfValidEquations = sumValidEquations(equations);

  std::cout << "Part 1: " << sumOfValidEquations << '\n';

  return 0;
}
