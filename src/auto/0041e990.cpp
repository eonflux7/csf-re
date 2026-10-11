#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original game heap free (cdecl, 1 stack arg).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Virtual deleting destructor signature: slot 0 of the entry object's vtable, __thiscall.
using EntryDeleteFn = void(__fastcall*)(void* self, void* edx, int deleteFlags);

static int __fastcall S_0041e990_ReleaseAllEntries(S_0041e990* self, void* /*edx*/)
{
    // Original count is captured once; the loop bound does not change while iterating.
    const uint32_t entryCount = self->field_000c;

    for (uint32_t index = 0; index < entryCount; ++index)
    {
        // Re-read the entry array each iteration: a callee may have changed it.
        uint32_t entryValue = reinterpret_cast<uint32_t*>(self->field_0004)[index];
        if (entryValue != 0)
        {
            void* entryObject = reinterpret_cast<void*>(static_cast<uintptr_t>(entryValue));
            // entryObject->vtbl[0](entryObject, 1): deleting destructor.
            EntryDeleteFn deleteEntry = reinterpret_cast<EntryDeleteFn>(CSF_AT(void**, entryObject, 0)[0]);
            deleteEntry(entryObject, nullptr, 1);

            // Clear the slot after the call (array reloaded from the object).
            reinterpret_cast<uint32_t*>(self->field_0004)[index] = 0;
        }
    }

    // Free the entry array itself.
    uint32_t entryArray = self->field_0004;
    self->field_0008 = 0;
    self->field_000c = 0;
    if (entryArray != 0)
    {
        Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(entryArray)));
        self->field_0004 = 0;
    }

    // Free the secondary buffer; its Mem_Free result is the function's return value.
    int result = static_cast<int>(self->field_0014);
    self->field_0018 = 0;
    self->field_001c = 0;
    if (result != 0)
    {
        result = Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0014)));
        self->field_0014 = 0;
    }
    return result;
}

HOOK(0x41E990, S_0041e990_ReleaseAllEntries, "auto");
