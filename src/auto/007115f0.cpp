#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint64_t, __cdecl, sub_7113E0, 0x007113E0, (float a1, int a2, int a3));

CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

// Converts a value in [-1, 1] to a DirectSound pan-style value and applies it
// through the interface stored at field_0004 (vtable slot 16). The original leaves the object in ecx and its
// vtable in edx (`mov ecx, [esi+4]`; `mov edx, [ecx]`) and pushes the value and the object; the __fastcall
// pointer below passes those registers and the same two stack arguments.
static int __fastcall C_00863cd4_SetNormalizedLevel(C_00863cd4* self, void* /*edx*/, float level)
{
    float v = level;
    bool useInverse = false;

    if (v <= g_TransitionOne) {
        if (v >= g_MissingMetricValue) {
            if (v > g_fZero) {
                useInverse = true;
            }
        } else {
            v = g_MissingMetricValue;
        }
    } else {
        v = g_TransitionOne;
        useInverse = true;
    }

    if (useInverse) {
        float arg = g_TransitionOne - v;
        int r = (int)sub_7113E0(arg, -10000, 0);
        void* obj = reinterpret_cast<void*>(self->field_0004);
        void* objVtbl = CSF_AT(void*, obj, 0);
        return ((int(__fastcall*)(void*, void*, void*, int))CSF_AT(void**, obj, 0)[0x40 / 4])(obj, objVtbl, obj, -r);
    }

    float arg = v + g_TransitionOne;
    int r = (int)sub_7113E0(arg, -10000, 0);
    void* obj = reinterpret_cast<void*>(self->field_0004);
    void* objVtbl = CSF_AT(void*, obj, 0);
    return ((int(__fastcall*)(void*, void*, void*, int))CSF_AT(void**, obj, 0)[0x40 / 4])(obj, objVtbl, obj, r);
}

HOOK(0x007115F0, C_00863cd4_SetNormalizedLevel, "auto");
