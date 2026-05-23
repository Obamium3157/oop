#ifndef OOP_FINDMAXEX_H
#define OOP_FINDMAXEX_H

#include <vector>

template <typename T, typename Less>
bool FindMaxEx(std::vector<T> const& arr, T& maxValue, Less const& less)
{
    if (arr.empty())
    {
        return false;
    }

    T const* bestCandidate = &arr[0];

    for (std::size_t index = 1; index < arr.size(); ++index)
    {
        if (less(*bestCandidate, arr[index]))
        {
            bestCandidate = &arr[index];
        }
    }

    maxValue = *bestCandidate;
    return true;
}

#endif //OOP_FINDMAXEX_H
