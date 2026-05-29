#ifndef OOP_FINDMAXEX_H
#define OOP_FINDMAXEX_H

#include <vector>

template <typename Less, typename T>
concept NoexceptComparator = requires(const Less& less, const T& lhs, const T& rhs)
{
    { less(lhs, rhs) } noexcept -> std::convertible_to<bool>;
};

template <typename Iter, NoexceptComparator<typename std::iterator_traits<Iter>::value_type> Less>
bool FindMaxEx(const Iter& first, const Iter& last, typename std::iterator_traits<Iter>::value_type& maxValue,
               const Less& less)
{
    if (first == last)
    {
        return false;
    }

    auto bestCandidate = *first;
    for (auto it = std::next(first); it != last; ++it)
    {
        if (less(bestCandidate, *it))
        {
            bestCandidate = *it;
        }
    }

    maxValue = bestCandidate;
    return true;
}

#endif //OOP_FINDMAXEX_H
