#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __cdecl, ClampContactComponent, 0x0040C920, (float* a1, int a2, int a3));

// Reads two floats from virtual getters (slots 0x30 then 0x2C), clamps them
// through ClampContactComponent against the component at this+0xE94, and
// returns that component's value.
static double __fastcall C_008300c0_ClampAndReadContactValue(C_008300c0* self, void* /*edx*/)
{
    float firstValue;   // [ebp-4]
    float secondValue;  // [ebp-8]

    firstValue = ((float(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x30 / 4])(self, nullptr);
    secondValue = ((float(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x2C / 4])(self, nullptr);

    float* contactComponent = reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(self) + 0xE94);
    ClampContactComponent(contactComponent,
                          static_cast<int>(reinterpret_cast<uintptr_t>(&secondValue)),
                          static_cast<int>(reinterpret_cast<uintptr_t>(&firstValue)));

    return CSF_AT(float, self, 0xE94);
}

HOOK(0x00429900, C_008300c0_ClampAndReadContactValue, "auto");
