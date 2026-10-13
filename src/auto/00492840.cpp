#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, GfxMatrix_EulerAnglesToDirection, 0x006AE640, (float* self, void* , float* out));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));

// Reads the Euler-angle source from the virtual slot at +0xAC, converts it to a
// direction vector stored in the sub-object at +0x1C8, normalizes it, and returns
// a pointer to that sub-object.
static uint32_t* __fastcall C_00833b88_UpdateDirectionFromEulerAngles(C_00833b88* self, void* /*edx*/)
{
    // Stack frame: [ebp-0x14] direction output (12 bytes), [ebp-0x8] angle output (8 bytes)
    alignas(8) uint8_t directionBytes[12];
    alignas(8) uint8_t angleBytes[8];

    using AngleSourceFn = void*(__fastcall*)(void* self, void* edx, uint32_t* out);
    void* angleSource = reinterpret_cast<AngleSourceFn>(CSF_AT(void**, self, 0)[0xAC / 4])(
        self, nullptr, reinterpret_cast<uint32_t*>(angleBytes));

    float* eulerDirection = GfxMatrix_EulerAnglesToDirection(
        static_cast<float*>(angleSource), nullptr, reinterpret_cast<float*>(directionBytes));

    uint8_t* vectorBase = reinterpret_cast<uint8_t*>(self) + 0x1C8;
    CSF_AT(uint32_t, vectorBase, 0) = reinterpret_cast<uint32_t*>(eulerDirection)[0];
    CSF_AT(uint32_t, vectorBase, 4) = reinterpret_cast<uint32_t*>(eulerDirection)[1];
    CSF_AT(uint32_t, vectorBase, 8) = reinterpret_cast<uint32_t*>(eulerDirection)[2];

    NormalizeVector3(reinterpret_cast<C_00833b88*>(vectorBase), nullptr);

    return reinterpret_cast<uint32_t*>(vectorBase);
}

HOOK(0x492840, C_00833b88_UpdateDirectionFromEulerAngles, "auto");
