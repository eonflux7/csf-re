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

CSF_FUNC(void, __fastcall, C_00862390_RefreshPhaseA, 0x006F1170, (void* self, void* edx, float a3));
CSF_FUNC(void, __fastcall, C_00862390_RefreshPhaseB, 0x006F0E90, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, C_00862390_RefreshPhaseC, 0x006F10C0, (void* self, void* edx, float a2));
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// __thiscall, one stack argument (ret 4).
static void __fastcall C_00862390_RefreshUnlessGlobalFlagged(C_00862390* self, void* /*edx*/, int a2)
{
    if (self->vCheckGlobalObjectFlag())
        return;

    C_00862390_RefreshPhaseA(self, nullptr, *(float*)&a2);
    C_00862390_RefreshPhaseB(self, nullptr, *(float*)&a2);
    C_00862390_RefreshPhaseC(self, nullptr, *(float*)&a2);

    if (self->field_0028 < g_fZero && self->field_0024 == g_fZero)
        self->vSlot2C(1);
}

HOOK(0x006F1380, C_00862390_RefreshUnlessGlobalFlagged, "auto");
