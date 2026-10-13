#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Calls the virtual method at slot 5 (offset 0x14) of the object stored at self+8,
// passing the value at self+16 and a 16-byte local buffer, then copies the four
// floats the callee filled into the caller's output array.
static float* __fastcall Float4_FetchFromSourceObject(void* self, void* /*edx*/, float* outValues)
{
    alignas(8) float fetchedValues[4];

    void* sourceObject = CSF_AT(void*, self, 0x08);
    int sourceParam = CSF_AT(int, self, 0x10);

    ((void(__fastcall*)(void*, void*, float*, int))CSF_AT(void**, sourceObject, 0)[5])(
        sourceObject, nullptr, fetchedValues, sourceParam);

    outValues[0] = fetchedValues[0];
    outValues[1] = fetchedValues[1];
    outValues[2] = fetchedValues[2];
    outValues[3] = fetchedValues[3];
    return outValues;
}

HOOK(0x0073BAC0, Float4_FetchFromSourceObject, "auto");
