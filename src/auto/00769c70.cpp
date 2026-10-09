#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

typedef unsigned char (__cdecl *SortPredicate)(int, int);

CSF_FUNC(int**, __cdecl, Sort_Partition, 0x00769840, (int** out, int* first, int* last, SortPredicate pred));
CSF_FUNC(int, __cdecl, Sort_HeapSortBuild, 0x00769A20, (int a1, int a2, int a3, int a4, int a5));
CSF_FUNC(int*, __cdecl, Sort_InsertionSort, 0x00769B40, (int a1, int* a2, SortPredicate pred));
CSF_FUNC(int, __cdecl, Sort_HeapSortFinish, 0x00769C20, (int* a1, int a2, SortPredicate pred));

static int __cdecl InputBindings_IntroSort(int* first, int* last, int depth, SortPredicate pred)
{
    int* range[2];
    int count = (int)(last - first);

    while (count > 32)
    {
        if (depth <= 0)
        {
            // Depth limit exhausted: fall back to heap sort
            if ((int)(((char*)last - (char*)first) & 0xFFFFFFFC) > 4)
                Sort_HeapSortBuild((int)first, (int)last, (int)pred, 0, 0);
            return Sort_HeapSortFinish(first, (int)last, pred);
        }

        Sort_Partition(range, first, last, pred);
        depth = depth / 2 / 2 + depth / 2;

        int leftSize = (int)(((char*)range[0] - (char*)first) & 0xFFFFFFFC);
        int rightSize = (int)(((char*)last - (char*)range[1]) & 0xFFFFFFFC);
        if (leftSize >= rightSize)
        {
            InputBindings_IntroSort(range[1], last, depth, pred);
            last = range[0];
        }
        else
        {
            InputBindings_IntroSort(first, range[0], depth, pred);
            first = range[1];
        }
        count = (int)(last - first);
    }

    if (count > 1)
        return (int)Sort_InsertionSort((int)first, last, pred);
    return count;
}

HOOK(0x00769C70, InputBindings_IntroSort, "auto");
