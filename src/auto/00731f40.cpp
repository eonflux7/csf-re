#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns true when the value is strictly greater than the last float stored
// in the list (begin at +0x1C, end at +0x20, 4-byte elements). Returns true
// when the list is empty; returns false when the flag at +0x28 is set.
static bool __fastcall C_00832eec_IsAboveLastThreshold(C_00832eec* self, void* /*edx*/, float value)
{
    if (self->field_0028)
        return false;

    uint32_t listBegin = self->field_001c;
    if (listBegin == 0)
        return true;

    uint32_t listEnd = self->field_0020;
    int elementCount = static_cast<int>(listEnd - listBegin) >> 2;
    if (elementCount == 0)
        return true;

    float lastElement = *reinterpret_cast<float*>(listEnd - 4);
    return value > lastElement;
}

HOOK(0x731F40, C_00832eec_IsAboveLastThreshold, "auto");
