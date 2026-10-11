#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ActionArgs_ReadPair, 0x004B7040, (void* self, void* edx, uint32_t* a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, ActionList_PushEntry, 0x004BC810, (void* self, void* edx, int a2, int a3));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));

// Vtable of the entry-array object (S_0041e5f0) referenced by this function's module.
[[maybe_unused]] static void* const s_EntryArrayVtable = CSF_VTBL(0x0041E5F0);

static bool __fastcall S_004bc930_AppendActionRecord20(S_004bc930* self, void* /*edx*/, int /*a2*/)
{
    uint32_t firstArg = 0;
    uint32_t storedValue = 0;
    ActionArgs_ReadPair(&self->field_00cc, nullptr, &firstArg, &storedValue);

    uint8_t* actionList = reinterpret_cast<uint8_t*>(self) + 0x6C;
    ActionList_PushEntry(actionList, nullptr, 20, static_cast<int>(firstArg));

    S_0041e5f0* entries = reinterpret_cast<S_0041e5f0*>(actionList + 0x10);
    if (CSF_AT(uint32_t, actionList, 0x18) >= CSF_AT(uint32_t, actionList, 0x14))
        S_0041e5f0_GrowArrayCapacity(entries, nullptr, static_cast<int>(entries->field_000c));

    uint32_t count = entries->field_0008;
    uint32_t* slot = reinterpret_cast<uint32_t*>(entries->field_0000 + 4 * count);
    entries->field_0008 = count + 1;
    *slot = storedValue;
    return true;
}

HOOK(0x004BFFE0, S_004bc930_AppendActionRecord20, "auto");
