#include <cstdint>
#include "globals.h"
#include "hook.h"

// Sift-down helper: restores the heap property below index i (a1 = range start, a3 = element count).
CSF_FUNC(int, __cdecl, InputBindings_SiftDownHeap, 0x769280,
         (int a1, int a2, int a3, int a4, unsigned char (__cdecl* a5)(int, int)));

// Builds a max-heap over the 4-byte element range [first, last) using the comparer at a3.
static int __cdecl InputBindings_MakeHeap(int first, int last, int comparer) {
    int count = (last - first) >> 2;
    // Signed adjustment as in the original: count + (count < 0 ? 1 : 0). Returned if the loop never runs.
    int result = count - (count >> 31);

    int i = count / 2;
    while (i > 0) {
        int value = *reinterpret_cast<int*>(first + 4 * i - 4);
        i--;
        result = InputBindings_SiftDownHeap(first, i, count, value,
                                            reinterpret_cast<unsigned char (__cdecl*)(int, int)>(comparer));
    }
    return result;
}

HOOK(0x769A20, InputBindings_MakeHeap, "auto");
