#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_004fc1e0_FindLookupIndex, 0x004FC1E0, (void* self, void* edx, int a2));

// Retained as required by the packet's vtable-reference list.
static const auto g_TableLookupVtable = CSF_VTBL(0x004FC1E0);

static void __fastcall S_004fc1e0_SetEntryLookup(
    S_004fc1e0* self, void* /*edx*/, int entryIndex, int lookupId)
{
    uint32_t lookupIndex = 0;
    if (lookupId != -1)
    {
        const uint32_t count = self->field_002c;
        if (count == 0)
            return;

        void** cursor = reinterpret_cast<void**>(self->field_0024);
        while (CSF_AT(int, *cursor, 0x0C) != lookupId)
        {
            ++lookupIndex;
            ++cursor;
            if (lookupIndex >= count)
                return;
        }

        if (lookupIndex == static_cast<uint32_t>(-1))
            return;
    }

    void* entry = reinterpret_cast<void**>(self->field_0014)[entryIndex];
    const int previousId = CSF_AT(int, entry, 0x10);
    if (previousId != -1)
    {
        const int previousIndex =
            S_004fc1e0_FindLookupIndex(self, nullptr, previousId);
        CSF_AT(int, entry, 0x10) = -1;

        void* resolved = reinterpret_cast<void*>(
            ResolveEntryLookupValue(
                reinterpret_cast<void**>(self->field_0014)[entryIndex],
                nullptr));

        void* previousLookup =
            reinterpret_cast<void**>(self->field_0024)[previousIndex];
        const uint32_t value = CSF_AT(uint32_t, previousLookup, 0);
        reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, resolved, 0)[0x64 / 4])(
                resolved, nullptr, value);
    }

    if (lookupId != -1)
    {
        CSF_AT(int,
            reinterpret_cast<void**>(self->field_0014)[entryIndex],
            0x10) = lookupId;

        void* resolved = reinterpret_cast<void*>(
            ResolveEntryLookupValue(
                reinterpret_cast<void**>(self->field_0014)[entryIndex],
                nullptr));

        void* lookup =
            reinterpret_cast<void**>(self->field_0024)[lookupIndex];
        const uint32_t value = CSF_AT(uint32_t, lookup, 0);
        reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, resolved, 0)[0x60 / 4])(
                resolved, nullptr, value);
    }
}

HOOK(0x004FCA10, S_004fc1e0_SetEntryLookup, "auto");
