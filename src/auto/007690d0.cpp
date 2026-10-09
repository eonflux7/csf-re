#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// std::rotate for a range of 32-bit elements (random access, gcd cycle algorithm).
static int __cdecl InputBindings_RotateDwords(int* first, int* middle, int* last)
{
    int shift = static_cast<int>(middle - first);
    int count = static_cast<int>(last - first);
    int result = count;
    int divisor = shift;

    if (shift != 0) {
        do {
            int rem = result % divisor;
            result = divisor;
            divisor = rem;
        } while (divisor != 0);
    }

    if (result < count && result > 0) {
        int* start = first + result;
        int remaining = result;
        do {
            int saved = *start;
            int* hole = start;
            int* next;
            if (start + shift == last)
                next = first;
            else
                next = start + shift;

            while (next != start) {
                *hole = *next;
                int tail = static_cast<int>(last - next);
                hole = next;
                if (shift >= tail)
                    next = first + (shift - tail);
                else
                    next = next + shift;
            }
            --start;
            result = remaining - 1;
            *hole = saved;
            remaining = result;
        } while (remaining != 0);
    }
    return result;
}

HOOK(0x007690D0, InputBindings_RotateDwords, "auto");
