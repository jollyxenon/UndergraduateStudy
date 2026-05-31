#include "polynomial_parser.hpp"
#include <sstream>
#include <stack>

// YOUR CODE HERE
Polynomial
PolynomialParser::compute_polynomial(const std::string &expression,
                                     std::map<std::string, Polynomial> &polys) {
  std::string temp;
  std::stringstream ss(expression);
  std::stack<Polynomial> operators;
  while (ss >> temp) {
    if (temp == "+" && operators.size() >= 2) {
      Polynomial p2 = operators.top();
      operators.pop();
      Polynomial p1 = operators.top();
      operators.pop();
      operators.push(p1 + p2);
    } else if (temp == "-" && operators.size() >= 2) {
      Polynomial p2 = operators.top();
      operators.pop();
      Polynomial p1 = operators.top();
      operators.pop();
      operators.push(p1 - p2);
    } else if (temp == "*" && operators.size() >= 2) {
      Polynomial p2 = operators.top();
      operators.pop();
      Polynomial p1 = operators.top();
      operators.pop();
      operators.push(p1 * p2);
    } else if ((temp == "+" || temp == "-" || temp == "*") &&
               operators.size() < 2) {
      throw std::invalid_argument(
          "Invalid expression: not enough operands for operator " + temp);
    } else {
      operators.push(polys[temp]);
    }
  }
  if (operators.size() != 1) {
    throw std::invalid_argument(
        "Invalid expression: too many operands or operators");
  }
  return operators.top();
}

PolynomialParser::scalarFct
PolynomialParser::compute_lambda(const std::string &expression,
                                 std::map<std::string, Polynomial> &polys) {
  Polynomial p = compute_polynomial(expression, polys);
  return [p](double x) -> double { return p(x); };
}