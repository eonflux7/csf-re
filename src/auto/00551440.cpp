#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());

// Notifies the global state object through its vtable slot at +0xE4 (slot 57).
static void MpSpawnPoint_NotifyGlobalStateVoid()
{
    int globalState = GetGlobalState_6A6D70();
    void* stateObj = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(globalState)));
    using VoidSlotFn = void(__fastcall*)(void*, void*);
    reinterpret_cast<VoidSlotFn>(CSF_AT(void**, stateObj, 0)[0xE4 / 4])(stateObj, nullptr);
}

// Same slot as above, but returns the slot's int result.
static int MpSpawnPoint_NotifyGlobalStateInt()
{
    int globalState = GetGlobalState_6A6D70();
    void* stateObj = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(globalState)));
    using IntSlotFn = int(__fastcall*)(void*, void*);
    return reinterpret_cast<IntSlotFn>(CSF_AT(void**, stateObj, 0)[0xE4 / 4])(stateObj, nullptr);
}

static int __fastcall MpSpawnPoint_ResetAndNotify(S_00551440* self, void* /*edx*/)
{
    if (self->field_1444 == 1)
    {
        MpSpawnPoint_NotifyGlobalStateVoid();
    }

    int result = static_cast<int>(self->field_1448) - 2;
    self->field_1444 = 0;
    if (result == 0)
    {
        result = MpSpawnPoint_NotifyGlobalStateInt();
    }

    self->field_1448 = 0;
    return result;
}

HOOK(0x00551440, MpSpawnPoint_ResetAndNotify, "auto");
