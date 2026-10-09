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
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(char, __fastcall, SearchFacingTarget, 0x0075CE00, (void* self, void* edx, float* a2, uint32_t* a3));
CSF_FUNC(double, __fastcall, GetFacingAngleRadians, 0x0075D0A0, (void* self, void* edx, uint32_t* a2));
CSF_GLOBAL(float, g_RadToDeg, 0x008313E4);

static void __stdcall WriteFacingAngleDegrees(int a1, int a2, int arg8)
{
    (void)a2;

    // Scene/manager object reached through the global accessor; the subsystem lives at +0x1FD8 (8152).
    void* root = reinterpret_cast<void*>(GetValue8B0554());
    C_0083e0d8* v3 = reinterpret_cast<C_0083e0d8*>(CSF_AT(uint32_t, root, 0x1FD8));

    uint32_t hitPair[2];
    hitPair[0] = 0xFFFFFFFFu;
    hitPair[1] = 0xFFFFFFFFu;

    // Position: *(*arg8 + 4) + 4
    void* argPtr = reinterpret_cast<void*>(arg8);
    void* argInner = reinterpret_cast<void*>(CSF_AT(uint32_t, argPtr, 0));
    float* positionPtr = reinterpret_cast<float*>(CSF_AT(uint32_t, argInner, 4) + 4);

    if (SearchFacingTarget(static_cast<void*>(v3), nullptr, positionPtr, hitPair))
    {
        double radians = GetFacingAngleRadians(static_cast<void*>(v3), nullptr, hitPair);
        float degrees = (float)(radians * g_RadToDeg);

        void* outObj = reinterpret_cast<void*>(CSF_AT(uint32_t, reinterpret_cast<void*>(a1), 4));
        CSF_AT(float, outObj, 4) = degrees;
    }
    else
    {
        void* outObj = reinterpret_cast<void*>(CSF_AT(uint32_t, reinterpret_cast<void*>(a1), 4));
        CSF_AT(uint32_t, outObj, 4) = 0;
    }
}

HOOK(0x0045C0C0, WriteFacingAngleDegrees, "auto");
