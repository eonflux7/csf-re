#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class initializer (C_0083e160 chain), __thiscall with 2 stack args.
CSF_FUNC(uint32_t*, __fastcall, C_0083e160_InitBase, 0x004E1200, (void* self, void* edx, int a2, int a3));
// Returns the current global state object (__cdecl, no args).
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
// Returns the menu transition alpha for the given state object (__thiscall, no args).
CSF_FUNC(float, __fastcall, MenuSystem_GetTransitionAlpha, 0x0050D810, (void* self, void* edx));

static int __fastcall C_0083e5e8_InitializeWithTransitionAlpha(C_0083e5e8* self, void* /*edx*/, int a2, int a3)
{
    C_0083e160_InitBase(self, nullptr, a2, a3);

    *(void***)self = CSF_VTBL(0x0083E5E8);
    self->field_0004 = (uint32_t)CSF_ADDR_DATA(0x0083E5CC);
    CSF_AT(int, self, 0x18) = 0;
    CSF_AT(void*, self, 0x20) = CSF_VTBL(0x0082D448);
    CSF_AT(int, self, 0x24) = -1;
    CSF_AT(int, self, 0x28) = -1;
    CSF_AT(uint8_t, self, 0x1C) = 0;
    CSF_AT(float, self, 0x2C) = 1.0f;

    int globalState = GetGlobalState_50D190();
    float transitionAlpha = MenuSystem_GetTransitionAlpha(reinterpret_cast<void*>(globalState), nullptr);
    float scaledCount = transitionAlpha * (float)(int)self->field_0008;
    self->field_0008 = (uint32_t)(int)scaledCount;

    CSF_AT(uint8_t, self, 0x30) = 0;
    return reinterpret_cast<int>(self);
}

HOOK(0x004E2110, C_0083e5e8_InitializeWithTransitionAlpha, "auto");
