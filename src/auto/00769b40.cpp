#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, InputBindings_RotateRange, 0x007690D0, (int a1, int a2, uint32_t* a3, int a4, int a5));

typedef uint8_t(__cdecl* CompareFn)(int, int);

// Insertion sort over an array of ints in [first, last) using a "less than" comparator.
static int* __cdecl InputBindings_InsertionSort(int a1, int* last, CompareFn less)
{
    int* result = last;
    int* sorted = reinterpret_cast<int*>(a1);
    if (reinterpret_cast<int*>(a1) != last)
    {
        for (int* cur = reinterpret_cast<int*>(a1) + 1; cur != last; ++cur)
        {
            if (less(*cur, *sorted))
            {
                if (sorted != cur && cur != cur + 1)
                    InputBindings_RotateRange(reinterpret_cast<int>(sorted), reinterpret_cast<int>(cur),
                                              reinterpret_cast<uint32_t*>(cur + 1), 0, 0);
            }
            else
            {
                int* prev = cur - 1;
                if (less(*cur, *(cur - 1)))
                {
                    int* pos;
                    int prevVal;
                    do
                    {
                        prevVal = *(prev - 1);
                        pos = prev;
                        --prev;
                    } while (less(*cur, prevVal));
                    if (pos != cur)
                        InputBindings_RotateRange(reinterpret_cast<int>(pos), reinterpret_cast<int>(cur),
                                                  reinterpret_cast<uint32_t*>(cur + 1), 0, 0);
                    sorted = reinterpret_cast<int*>(a1);
                }
            }
            result = last;
        }
    }
    return result;
}

HOOK(0x00769B40, InputBindings_InsertionSort, "auto");
