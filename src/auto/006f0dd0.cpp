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

// Callee: C_00860a30 method (__thiscall, one stack arg, ret 4)
CSF_FUNC(void, __fastcall, C_00860a30_SetFlag0C, 0x006C9FE0, (C_00860a30* self, void* /*edx*/, int flag));

// Data: zero constant used by the comparisons
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Sets the derived ratio fields (+0x28, +0x2C) from the source value at
// field_0018->+0x5C and field_0024, or, when the inputs are invalid or a2 is set,
// resets the value at field_0020 and notifies the child at field_0034.
static void __fastcall C_00862390_UpdateRatioOrReset(C_00862390* self, void* /*edx*/, char a2)
{
    bool takeReset = (a2 != 0);
    uint32_t sourceObj = 0;

    if (!takeReset)
    {
        sourceObj = self->field_0018;
        takeReset = (CSF_AT(float, reinterpret_cast<uint8_t*>(sourceObj), 0x5C) == g_fZero);
    }
    if (!takeReset)
    {
        takeReset = (self->field_0024 == g_fZero);
    }

    if (takeReset)
    {
        // field_0020->vSetValue3d(0)  (slot +0x2C)
        void* valueObj = reinterpret_cast<void*>(self->field_0020);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(CSF_AT(void**, valueObj, 0)[0x2C / 4])(valueObj, nullptr, 0);

        // if (field_0034) field_0034->vtbl[+0x1C]()
        uint32_t child = self->field_0034;
        if (child != 0)
        {
            void* childObj = reinterpret_cast<void*>(child);
            reinterpret_cast<void(__fastcall*)(void*, void*)>(CSF_AT(void**, childObj, 0)[0x1C / 4])(childObj, nullptr);
        }

        C_00860a30_SetFlag0C(reinterpret_cast<C_00860a30*>(self), nullptr, 1);
    }
    else
    {
        float denominator = CSF_AT(float, reinterpret_cast<uint8_t*>(sourceObj), 0x5C);
        self->field_002c = CSF_AT(float, reinterpret_cast<uint8_t*>(sourceObj), 0x5C);
        self->field_0028 = -self->field_0024 / denominator;
    }
}

HOOK(0x6F0DD0, C_00862390_UpdateRatioOrReset, "auto");
