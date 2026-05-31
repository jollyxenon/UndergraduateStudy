#include "polynomial.hpp"
#include <fstream>

// YOUR CODE HERE
Polynomial::Polynomial() : m_coeffs{0.0} {}

Polynomial::Polynomial(const std::vector<double> &coeffs) : m_coeffs(coeffs) {}
Polynomial::Polynomial(std::vector<double> &&coeffs)
    : m_coeffs(std::move(coeffs)) {}
Polynomial::Polynomial(std::initializer_list<double> coeffs)
    : m_coeffs(coeffs) {}

Polynomial::Polynomial(const std::string &path) {
  // read coefficients from file
  std::ifstream infile(path);
  double coeff;
  while (infile >> coeff) {
    m_coeffs.push_back(coeff);
  }
}

Polynomial::Polynomial(const Polynomial &p) : m_coeffs(p.m_coeffs) {}
Polynomial::Polynomial(Polynomial &&p) noexcept
    : m_coeffs(std::move(p.m_coeffs)) {}

Polynomial &Polynomial::operator=(const Polynomial &p) {
  if (this != &p) {
    m_coeffs = p.m_coeffs;
  }
  return *this;
}
Polynomial &Polynomial::operator=(Polynomial &&p) noexcept {
  if (this != &p) {
    m_coeffs = std::move(p.m_coeffs);
  }
  return *this;
}

Polynomial::~Polynomial() {}

double &Polynomial::operator[](int index) { return m_coeffs[index]; }
const double &Polynomial::operator[](int index) const {
  return m_coeffs[index];
}

int Polynomial::size() const { return m_coeffs.size(); }

Polynomial &Polynomial::operator+=(const Polynomial &p) {
  if (p.size() > size()) {
    m_coeffs.resize(p.size(), 0.0);
  }
  for (int i = 0; i < p.size(); ++i) {
    m_coeffs[i] += p.m_coeffs[i];
  }
  return *this;
}
Polynomial Polynomial::operator+(const Polynomial &p) const {
  Polynomial result(*this);
  result += p;
  return result;
}
Polynomial &Polynomial::operator-=(const Polynomial &p) {
  if (p.size() > size()) {
    m_coeffs.resize(p.size(), 0.0);
  }
  for (int i = 0; i < p.size(); ++i) {
    m_coeffs[i] -= p.m_coeffs[i];
  }
  return *this;
}
Polynomial Polynomial::operator-(const Polynomial &p) const {
  Polynomial result(*this);
  result -= p;
  return result;
}
Polynomial &Polynomial::operator*=(const Polynomial &p) {
  std::vector<double> result(size() + p.size() - 1, 0.0);
  for (int i = 0; i < size(); ++i) {
    for (int j = 0; j < p.size(); ++j) {
      result[i + j] += m_coeffs[i] * p.m_coeffs[j];
    }
  }
  m_coeffs = std::move(result);
  return *this;
}
Polynomial Polynomial::operator*(const Polynomial &p) const {
  Polynomial result(*this);
  result *= p;
  return result;
}
Polynomial Polynomial::operator*(double factor) const {
  Polynomial result(*this);
  for (auto &coeff : result.m_coeffs) {
    coeff *= factor;
  }
  return result;
}

double Polynomial::operator()(double x) const {
  double result = 0.0;
  double power = 1.0;
  for (const auto &coeff : m_coeffs) {
    result += coeff * power;
    power *= x;
  }
  return result;
}