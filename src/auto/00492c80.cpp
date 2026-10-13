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

CSF_FUNC(char, __fastcall, C_00833908_UpdateComponents, 0x004910C0, (C_00833908* self, void* , float deltaTime));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(void, __fastcall, sub_492280, 0x00492280, (void* self, void* edx, int a3));
CSF_FUNC(void, __fastcall, S_0048c9f0_UpdateTargetPoints, 0x0048D0F0, (S_0048c9f0* self, void* , float elapsedTime));

// Reinterprets the raw dword argument as the float the callees expect.
static float RawDwordToFloat(int raw)
{
    float value;
    std::memcpy(&value, &raw, sizeof(value));
    return value;
}

// Per-frame state update for C_00833b88: updates base components, then, when the
// 0x10 flag is set and the object is active, runs the derived update slot, the
// helper, and the target-point update.
static void __fastcall C_00833b88_UpdateState(C_00833b88* self, void* /*edx*/, int a2)
{
    C_00833908_UpdateComponents(self, nullptr, RawDwordToFloat(a2));

    if ((self->field_0170 & 0x10) != 0 && !SceneObj_IsInactive(self, nullptr))
    {
        CSF_AT(uint16_t, self, 0x208) = 0xFFFF;
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[0xC8 / 4])(self, nullptr, a2);
        sub_492280(self, nullptr, a2);

        S_0048c9f0* targetPoints = reinterpret_cast<S_0048c9f0*>(self->field_0204);
        if (targetPoints)
            S_0048c9f0_UpdateTargetPoints(targetPoints, nullptr, RawDwordToFloat(a2));
    }
}

HOOK(0x00492C80, C_00833b88_UpdateState, "auto");
