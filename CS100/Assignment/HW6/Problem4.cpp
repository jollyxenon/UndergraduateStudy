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
    // Type Aliases
    using size_type = std::size_t;
    using value_type = int;
    using pointer = int *;
    using reference = int &;
    using const_pointer = const int *;
    using const_reference = const int &;

    // Constructors
    explicit Dynarray(Dynarray::size_type n) : begin(new Dynarray::value_type[n]), end(begin + n)
    {
        for (Dynarray::value_type *p = begin; p != end; ++p)
            *p = 0;
    }
    Dynarray() : begin(nullptr), end(nullptr) {}
    Dynarray(const Dynarray::size_type n, const Dynarray::value_type x) : begin(new Dynarray::value_type[n]), end(begin + n)
    {
        for (Dynarray::value_type *p = begin; p != end; ++p)
            *p = x;
    }
    Dynarray(const Dynarray::value_type *begin_, const Dynarray::value_type *end_) : begin(new Dynarray::value_type[end_ - begin_]), end(begin + (end_ - begin_))
    {
        Dynarray::value_type *p = begin;
        const Dynarray::value_type *q = begin_;
        for (; q != end_; ++p, ++q)
            *p = *q;
    }
    Dynarray(const Dynarray &other) : begin(new Dynarray::value_type[other.size()]), end(begin + other.size())
    {
        Dynarray::value_type *p = begin;
        const Dynarray::value_type *q = other.begin;
        for (; q != other.end; ++p, ++q)
            *p = *q;
    }
    Dynarray(Dynarray &&other) noexcept : begin(other.begin), end(other.end)
    {
        other.begin = nullptr;
        other.end = nullptr;
    }

    // Destructor
    ~Dynarray() { delete[] begin; }

    // Copy and Move Assignment Operators
    Dynarray &operator=(const Dynarray &other)
    {
        if (this != &other)
        {
            Dynarray::value_type *new_begin = new Dynarray::value_type[other.size()];
            for (Dynarray::value_type *p = new_begin, *q = other.begin; q != other.end; ++p, ++q)
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

    // Member Functions

    /// @brief Returns the number of elements in the Dynarray.
    /// @return The size of the Dynarray.
    Dynarray::size_type size() const
    {
        return end - begin;
    }

    /// @brief Checks if the Dynarray is empty.
    /// @return true if the Dynarray is empty, false otherwise.
    bool empty() const
    {
        return size() == 0;
    }

    /// @brief Returns a reference to the element at the specified index.
    /// @param i The index of the element to return.
    /// @return A reference to the element at the specified index.
    Dynarray::value_type &at(Dynarray::size_type i)
    {
        if (i >= size())
            throw std::out_of_range("Dynarray index out of range!");
        return begin[i];
    }

    /// @brief Returns a const reference to the element at the specified index.
    /// @param i The index of the element to return.
    /// @return A const reference to the element at the specified index.
    const Dynarray::value_type &at(Dynarray::size_type i) const
    {
        if (i >= size())
            throw std::out_of_range("Dynarray index out of range!");
        return begin[i];
    }

    // Reloaded Operators
    /// @brief Provides array subscript operator for non-const Dynarray.
    /// @param i The index of the element to access.
    /// @return A reference to the element at the specified index.
    Dynarray::value_type &operator[](Dynarray::size_type i) { return begin[i]; }

    /// @brief Provides array subscript operator for const Dynarray.
    /// @param i The index of the element to access.
    /// @return A const reference to the element at the specified index.
    const Dynarray::value_type &operator[](Dynarray::size_type i) const { return begin[i]; }

    /// @brief Compares two Dynarray objects for equality.
    /// @param other The Dynarray to compare with.
    /// @return true if the Dynarray objects are equal, false otherwise.
    bool operator==(const Dynarray &other) const
    {
        return std::equal(begin, end, other.begin, other.end);
    }

    /// @brief Compares two Dynarray objects for less-than relationship.
    /// @param other The Dynarray to compare with.
    /// @return true if the first Dynarray is less than the second, false otherwise.
    bool operator<(const Dynarray &other) const
    {
        return std::lexicographical_compare(begin, end, other.begin, other.end);
    }

    bool operator>(const Dynarray &other) const { return other < *this; }
    bool operator!=(const Dynarray &other) const { return !(*this == other); }
    bool operator<=(const Dynarray &other) const { return !(*this > other); }
    bool operator>=(const Dynarray &other) const { return !(*this < other); }

    /// @brief Outputs the elements of the Dynarray to an ostream.
    /// @param os The ostream to output to.
    /// @param arr The Dynarray to output.
    /// @return The ostream after outputting the Dynarray.
    friend std::ostream &operator<<(std::ostream &os, const Dynarray &arr)
    {
        os << "[";
        for (Dynarray::size_type i = 0; i < arr.size(); ++i)
        {
            os << arr.begin[i];
            if (i != arr.size() - 1)
                os << ", ";
        }
        os << "]";
        return os;
    }
};
