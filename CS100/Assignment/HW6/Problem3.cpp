#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>

/**
 * @brief Given a vector of strings @c strings, reduce this vector so that each
 * string appears only once. After the function call, @c strings contains the
 * same set of elements (possibly sorted) as the original vector, but each
 * element appears only once.
 * * @note You are not allowed to write loops or recursions.
 */
void dropDuplicates(std::vector<std::string> &strings)
{
    std::sort(strings.begin(), strings.end());
    auto newEnd = std::unique(strings.begin(), strings.end());
    strings.erase(newEnd, strings.end());
}

/**
 * @brief Given a vector of strings @c strings and a nonnegative integer @c k,
 * partition the vector into two parts so that the first part contains
 * all the strings with length no greater than @c k.
 * * @return An iterator just past the last element of the first part.
 * @note You are not allowed to write loops or recursions.
 */
auto partitionByLength(std::vector<std::string> &strings, std::size_t k)
{
    return std::partition(strings.begin(), strings.end(),
                          [k](const std::string &s)
                          {
                              return s.size() <= k;
                          });
}

/**
 * @brief Generate all the permutations of {1, 2, ..., n} in lexicographical
 * order, and print them to @c os.
 *
 */
void generatePermutations(int n, std::ostream &os)
{
    std::vector<int> numbers(n);

    std::iota(numbers.rbegin(), numbers.rend(), 1);

    do
    {
        std::for_each(numbers.begin(), numbers.end(),
                      [&os](int x)
                      { os << x << ' '; });

        os << '\n';
    } while (std::prev_permutation(numbers.begin(), numbers.end()));
}