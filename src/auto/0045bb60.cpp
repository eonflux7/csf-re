#include "hook.h"

static int __cdecl ClampIntegerToUnorderedBounds(
    int* value, int* firstBound, int* secondBound)
{
    const int first = *firstBound;
    const int second = *secondBound;
    const int current = *value;

    if (first > second)
    {
        if (current < second)
            *value = second;
        else if (current > first)
            *value = first;
    }
    else
    {
        if (current < first)
            *value = first;
        else if (current > second)
            *value = second;
    }

    return first;
}

HOOK(0x0045BB60, ClampIntegerToUnorderedBounds, "auto");
