#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "hook.h"

// Accumulates a scaled vector into one 28-byte entry of the array at self+0x0C.
// If the entry's first dword is a non-null object, forwards the scaled vector
// to that object's virtual slot 17 (byte offset 0x44).
static int __fastcall ApplyScaledDeltaToElement(void* self, void* /*edx*/, int a2, float a3)
{
    uint8_t* elem = CSF_AT(uint8_t*, self, 0x0C) + 28 * a2;
    int result = static_cast<int>(reinterpret_cast<uintptr_t>(elem));

    // Source vector lives at (element+8)->+0x10.
    float* src = reinterpret_cast<float*>(CSF_AT(uint8_t*, elem, 0x08) + 0x10);
    float srcX = src[0];
    float srcY = src[1];
    float srcZ = src[2];

    float scaledX = srcX * a3;
    float scaledY = srcY * a3;
    float scaledZ = srcZ * a3;

    // Accumulate into the element's own vector at +0x10/+0x14/+0x18.
    CSF_AT(float, elem, 0x10) = scaledX + CSF_AT(float, elem, 0x10);
    CSF_AT(float, elem, 0x14) = scaledY + CSF_AT(float, elem, 0x14);
    CSF_AT(float, elem, 0x18) = scaledZ + CSF_AT(float, elem, 0x18);

    // Re-read the element's object pointer after the writes.
    void* obj = CSF_AT(void*, elem, 0x00);
    if (obj != nullptr)
    {
        float scaled[3] = { scaledX, scaledY, scaledZ };
        return ((int(__fastcall*)(void*, void*, float*))CSF_AT(void**, obj, 0)[17])(obj, nullptr, scaled);
    }
    return result;
}

HOOK(0x408370, ApplyScaledDeltaToElement, "auto");
