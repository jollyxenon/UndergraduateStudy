#include <cstddef>
#include <iostream>
#include <utility>
#include <algorithm>
#include <stdexcept>

class Dynarray
{
    int *begin;
    int *end;

public:
    explicit Dynarray(std::size_t n) : begin(new int[n]), end(begin + n)
    {
        for (int *p = begin; p != end; ++p)
            *p = 0;
    }
    Dynarray() : begin(nullptr), end(nullptr) {}
    Dynarray(const std::size_t n, const int x) : begin(new int[n]), end(begin + n)
    {
        for (int *p = begin; p != end; ++p)
            *p = x;
    }
    Dynarray(const int *begin_, const int *end_) : begin(new int[end_ - begin_]), end(begin + (end_ - begin_))
    {
        int *p = begin;
        const int *q = begin_;
        for (; q != end_; ++p, ++q)
            *p = *q;
    }
    Dynarray(const Dynarray &other) : begin(new int[other.size()]), end(begin + other.size())
    {
        int *p = begin;
        const int *q = other.begin;
        for (; q != other.end; ++p, ++q)
            *p = *q;
    }
    Dynarray(Dynarray &&other) noexcept : begin(other.begin), end(other.end)
    {
        other.begin = nullptr;
        other.end = nullptr;
    }

    ~Dynarray() { delete[] begin; }

    Dynarray &operator=(const Dynarray &other)
    {
        if (this != &other)
        {
            int *new_begin = new int[other.size()];
            for (int *p = new_begin, *q = other.begin; q != other.end; ++p, ++q)
                *p = *q;
            delete[] begin;
            begin = new_begin;
            end = begin + other.size();
        }
        return *this;
    }
    Dynarray &operator=(Dynarray &&other) noexcept
    {
        if (this != &other)
        {
            delete[] begin;
            begin = other.begin;
            end = other.end;
            other.begin = nullptr;
            other.end = nullptr;
        }
        return *this;
    }

    std::size_t size() const
    {
        return end - begin;
    }

    bool empty() const
    {
        return size() == 0;
    }

    int &at(std::size_t i)
    {
        if (i >= size())
            throw std::out_of_range("Dynarray index out of range!");
        return begin[i];
    }

    const int &at(std::size_t i) const
    {
        if (i >= size())
            throw std::out_of_range("Dynarray index out of range!");
        return begin[i];
    }
};

void reverse(Dynarray &a)
{
    for (int i = 0, j = a.size() - 1; i < j; ++i, --j)
        std::swap(a.at(i), a.at(j));
}

void print(const Dynarray &a)
{
    std::cout << '[';
    if (!a.empty())
    {
        for (std::size_t i = 0; i + 1 < a.size(); ++i)
            std::cout << a.at(i) << ", ";
        std::cout << a.at(a.size() - 1);
    }
    std::cout << ']' << std::endl;
}

int main()
{
    int n;
    std::cin >> n;
    Dynarray arr(n);
    for (int i = 0; i != n; ++i)
        std::cin >> arr.at(i);
    reverse(arr);
    print(arr);
    Dynarray copy = arr;
    copy.at(0) = 42;
    std::cout << arr.at(0) << '\n'
              << copy.at(0) << std::endl;
    return 0;
}