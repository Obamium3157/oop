#ifndef OOP_FINDMAXEX_H
#define OOP_FINDMAXEX_H

template <typename Less, typename T>
concept NoexceptComparator = requires(const Less& less, const T& lhs, const T& rhs)
{
    { less(lhs, rhs) } noexcept -> std::convertible_to<bool>;
};

template <typename Iter, NoexceptComparator<typename std::iterator_traits<Iter>::value_type> Less>
Iter FindMaxEx(Iter first, Iter last, const Less& less)
{
    if (first == last)
    {
        return last;
    }

    auto bestCandidate = first;
    for (auto it = std::next(first); it != last; ++it)
    {
        if (less(*bestCandidate, *it))
        {
            bestCandidate = it;
        }
    }

    return bestCandidate;
}

#endif //OOP_FINDMAXEX_H
