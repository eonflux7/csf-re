#include <cstdint>
#include "globals.h"
#include "hook.h"

using SortPredicateFn = bool (__cdecl*)(int, int);

CSF_FUNC(int, __cdecl, InputBindings_SortMedian, 0x007691D0, (int* first, int* mid, int* last, SortPredicateFn pred));

static int** __cdecl InputBindings_PartitionSortRange(
    int** result, unsigned int firstAddress, int* last, SortPredicateFn pred)
{
    const std::int32_t count =
        static_cast<std::int32_t>(
            reinterpret_cast<std::uintptr_t>(last) - firstAddress) >> 2;
    int* equalFirst = reinterpret_cast<int*>(firstAddress + 4 * (count / 2));

    InputBindings_SortMedian(
        reinterpret_cast<int*>(firstAddress), equalFirst, last - 1, pred);

    int* equalLast = equalFirst + 1;
    while (firstAddress < reinterpret_cast<std::uintptr_t>(equalFirst))
    {
        if (pred(*(equalFirst - 1), *equalFirst))
            break;
        if (pred(*equalFirst, *(equalFirst - 1)))
            break;
        --equalFirst;
    }

    while (reinterpret_cast<std::uintptr_t>(equalLast) <
           reinterpret_cast<std::uintptr_t>(last))
    {
        if (pred(*equalLast, *equalFirst))
            break;
        if (pred(*equalFirst, *equalLast))
            break;
        ++equalLast;
    }

    int* leftScan = equalFirst;
    int* rightScan = equalLast;

    for (;;)
    {
        while (reinterpret_cast<std::uintptr_t>(rightScan) <
               reinterpret_cast<std::uintptr_t>(last))
        {
            if (!pred(*equalFirst, *rightScan))
            {
                if (pred(*rightScan, *equalFirst))
                    break;

                const int saved = *equalLast;
                *equalLast = *rightScan;
                ++equalLast;
                *rightScan = saved;
            }
            ++rightScan;
        }

        if (reinterpret_cast<std::uintptr_t>(leftScan) > firstAddress)
        {
            int* candidate = leftScan - 1;
            do
            {
                if (!pred(*candidate, *equalFirst))
                {
                    if (pred(*equalFirst, *candidate))
                        break;

                    --equalFirst;
                    const int saved = *equalFirst;
                    *equalFirst = *candidate;
                    *candidate = saved;
                }
                --candidate;
                --leftScan;
            }
            while (firstAddress < reinterpret_cast<std::uintptr_t>(leftScan));
        }

        if (reinterpret_cast<std::uintptr_t>(leftScan) == firstAddress)
        {
            if (rightScan == last)
                break;

            if (equalLast != rightScan)
            {
                const int saved = *equalFirst;
                *equalFirst = *equalLast;
                *equalLast = saved;
            }

            const int saved = *equalFirst;
            *equalFirst = *rightScan;
            int* movedFrom = rightScan;
            ++equalLast;
            ++rightScan;
            ++equalFirst;
            *movedFrom = saved;
        }
        else
        {
            --leftScan;
            if (rightScan == last)
            {
                --equalFirst;
                if (leftScan != equalFirst)
                {
                    const int saved = *leftScan;
                    *leftScan = *equalFirst;
                    *equalFirst = saved;
                }

                const int replacement = *(equalLast - 1);
                const int saved = *equalFirst;
                --equalLast;
                *equalFirst = replacement;
                *equalLast = saved;
            }
            else
            {
                const int saved = *rightScan;
                *rightScan = *leftScan;
                ++rightScan;
                *leftScan = saved;
            }
        }
    }

    result[0] = equalFirst;
    result[1] = equalLast;
    return result;
}

HOOK(0x00769840, InputBindings_PartitionSortRange, "auto");
