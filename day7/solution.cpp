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

    equation.solution = std::stoull(lineToken.substr(0, delimIndex));

    std::istringstream termTokenizer(
        lineToken.substr(delimIndex + 2, lineToken.size() - 1));
    std::string termToken;

    while (std::getline(termTokenizer, termToken, ' ')) {
      equation.terms.push_back(std::stoull(termToken));
    }

    equations.push_back(equation);
  }

  return equations;
}

unsigned long long
evaluateExpression(const std::vector<unsigned long long> &expression,
                   const std::vector<char> &operators) {
  unsigned long long result = expression[0];

  for (size_t i = 0; i < operators.size(); ++i) {
    switch (operators[i]) {
    case '+':
      result += expression[i + 1];
      break;
    case '*':
      result *= expression[i + 1];
      break;
    case '|': {
      std::string concatenated =
          std::to_string(result) + std::to_string(expression[i + 1]);
      result = std::stoull(concatenated);
      break;
    }
    default:
      throw std::runtime_error("Invalid operator in evaluateExpression.");
    }
  }

  return result;
}

bool checkSolution(const Equation &equation, std::vector<char> &operators,
                   size_t index) {
  if (index == equation.terms.size() - 1) {
    return evaluateExpression(equation.terms, operators) == equation.solution;
  }

  operators[index] = '+';
  if (checkSolution(equation, operators, index + 1))
    return true;

  operators[index] = '*';
  return checkSolution(equation, operators, index + 1);
}

bool checkConcatSolution(const Equation &equation, std::vector<char> &operators,
                         size_t index) {
  if (index == equation.terms.size() - 1) {
    // Evaluate the expression with the current operator configuration
    unsigned long long result = evaluateExpression(equation.terms, operators);
    if (result == equation.solution) {
      // Debug output to show the successful operator sequence
      std::cout << "Valid: ";
      for (char op : operators) {
        std::cout << op << ' ';
      }
      std::cout << " -> " << result << '\n';
      return true;
    }
    return false;
  }

  // Try '+' operator
  operators[index] = '+';
  if (checkConcatSolution(equation, operators, index + 1)) {
    return true;
  }

  // Try '*' operator
  operators[index] = '*';
  if (checkConcatSolution(equation, operators, index + 1)) {
    return true;
  }

  // Try '|' (concatenation) operator
  operators[index] = '|';
  if (checkConcatSolution(equation, operators, index + 1)) {
    return true;
  }

  return false;
}

bool isValidEquation(const Equation &equation) {
  std::vector<char> operators(equation.terms.size() - 1);
  return checkSolution(equation, operators, 0);
}

bool isValidConcatEquation(const Equation &equation) {
  std::vector<char> operators(equation.terms.size() - 1);
  return checkConcatSolution(equation, operators, 0);
}

unsigned long long sumValidEquations(const std::vector<Equation> &equations) {
  unsigned long long sum = 0;

  for (const Equation &equation : equations) {
    if (isValidEquation(equation))
      sum += equation.solution;
  }

  return sum;
}

unsigned long long
sumConcatValidEquations(const std::vector<Equation> &equations) {
  unsigned long long sum = 0;

  for (const Equation &equation : equations)
    if (isValidConcatEquation(equation))
      sum += equation.solution;
    else
      printEquation(equation);

  return sum;
}

int main() {
  const std::string input = AOCHelper::readInput("Day7/input.txt");

  const std::vector<Equation> equations = parseInput(input);

  const unsigned long long sumOfValidEquations = sumValidEquations(equations);
  const unsigned long long sumOfValidEquationsWithConcat =
      sumConcatValidEquations(equations);

  std::cout << "Part 1: " << sumOfValidEquations << '\n';
  std::cout << "Part 2: " << sumOfValidEquationsWithConcat << '\n';

  return 0;
}
