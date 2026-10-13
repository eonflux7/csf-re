#include <cstdint>
#include <cstddef>

#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(void, __fastcall, RpPrtStd_ResetBaseState, 0x006C9FC0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, RpPrtStd_ReleaseParentState, 0x006C9F40, (void* self, void* edx));

static void __fastcall C_00862390_ReleaseLightAndParentState(C_00862390* self, void* /*edx*/)
{
    uint32_t lightObjectHandle = self->field_0020;

    self->vtbl = CSF_VTBL(0x862390);
    self->field_0004 = (uint32_t)(uintptr_t)CSF_VTBL(0x862384);
    self->field_0008 = (uint32_t)(uintptr_t)CSF_VTBL(0x862368);

    if (lightObjectHandle != 0)
    {
        int globalInt = GetGlobalInt();
        int manager = ResolveClumpManager(globalInt, nullptr);
        void** managerObj = reinterpret_cast<void**>(static_cast<uintptr_t>(manager));

        // Virtual slot +0x2C (index 11) on the clump manager: releases the light object.
        ((void(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, managerObj, 0)[11])(
            managerObj, nullptr, self->field_0020);
        self->field_0020 = 0;
    }

    uint32_t stateHandle = self->field_0034;
    if (stateHandle != 0)
    {
        void** stateObj = reinterpret_cast<void**>(static_cast<uintptr_t>(stateHandle));

        // Virtual slot +0x00 (index 0) with argument 1.
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, stateObj, 0)[0])(stateObj, nullptr, 1);
        self->field_0034 = 0;
    }

    RpPrtStd_ResetBaseState(self, nullptr);
    RpPrtStd_ReleaseParentState(self, nullptr);
}

HOOK(0x006F0F40, C_00862390_ReleaseLightAndParentState, "auto");
