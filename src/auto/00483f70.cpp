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

CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(char, __fastcall, ObjectProvider_SetPendingRequest, 0x004F5EF0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ObjectProvider_SetUpdateMode, 0x004F7810, (void* self, void* edx, int a3));

// Original: sub_483F70, __thiscall, ret 0.
// Resets the weapon FSM's owned state object to mode 1, then, if a cached
// object provider exists, queues a pending request on the state's sub-object
// (+0x1C) and clears the provider's update mode.
static C_00861120* __fastcall CCharActionFsm_SetIdleAndQueueProvider(CCharActionFsm* self, void* /*edx*/)
{
    void* stateObject = reinterpret_cast<void*>(self->field_0008);

    // stateObject->vSlot6C(1): virtual slot 0x6C / 4 = 27
    ((void(__fastcall*)(void*, void*, int))(CSF_AT(void**, stateObject, 0)[0x6C / 4]))(stateObject, nullptr, 1);

    int globalValue = GetGlobalValue_4FBDE0();
    int firstElement = GetFirstElement(reinterpret_cast<void*>(globalValue), nullptr);
    C_00861120* provider = reinterpret_cast<C_00861120*>(
        GetCachedObjectProvider(reinterpret_cast<void*>(firstElement), nullptr));

    if (provider) {
        int requestSlot = static_cast<int>(reinterpret_cast<uintptr_t>(
            reinterpret_cast<uint8_t*>(stateObject) + 0x1C));
        ObjectProvider_SetPendingRequest(provider, nullptr, requestSlot);
        return reinterpret_cast<C_00861120*>(ObjectProvider_SetUpdateMode(provider, nullptr, 0));
    }
    return provider;
}

HOOK(0x483F70, CCharActionFsm_SetIdleAndQueueProvider, "auto");
