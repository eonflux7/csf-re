#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Looks up a record through the field_0008 service object: resolves handle 2100,
// fetches the entry, and copies its three 32-bit values into the caller's buffer.
// If the lookup returns -1, the buffer is filled with zeros.
static uint32_t* __fastcall C_0083ec48_GetTripleFromLookup(C_0083ec48* self, void* /*edx*/, uint32_t* a2)
{
    void* lookupService = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0008));

    // lookupService->vtbl[10]() -> manager object
    void* manager = reinterpret_cast<void*(__fastcall*)(void*, void*)>(CSF_AT(void**, lookupService, 0)[10])(lookupService, nullptr);

    // manager->vtbl[38](2100) -> entry index
    int entryIndex = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(CSF_AT(void**, manager, 0)[38])(manager, nullptr, 2100);

    int value0 = 0;
    int value1 = 0;
    int value2 = 0;

    if (entryIndex != -1)
    {
        // lookupService->vtbl[10]() -> manager again
        void* manager2 = reinterpret_cast<void*(__fastcall*)(void*, void*)>(CSF_AT(void**, lookupService, 0)[10])(lookupService, nullptr);

        // manager2->vtbl[35](entryIndex) -> entry
        void* entry = reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(CSF_AT(void**, manager2, 0)[35])(manager2, nullptr, entryIndex);

        // entry->vtbl[6]() -> record
        void* record = reinterpret_cast<void*(__fastcall*)(void*, void*)>(CSF_AT(void**, entry, 0)[6])(entry, nullptr);

        // record->vtbl[4]() -> int* triple
        int* triple = reinterpret_cast<int*>(
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(CSF_AT(void**, record, 0)[4])(record, nullptr));

        value0 = triple[0];
        value1 = triple[1];
        value2 = triple[2];
    }

    a2[0] = static_cast<uint32_t>(value0);
    a2[1] = static_cast<uint32_t>(value1);
    a2[2] = static_cast<uint32_t>(value2);
    return a2;
}

HOOK(0x4E8B10, C_0083ec48_GetTripleFromLookup, "auto");
