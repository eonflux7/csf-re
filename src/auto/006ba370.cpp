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

// Callees (bound by address)
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, sub_6B9580, 0x006B9580, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(char, __fastcall, sub_6BA300, 0x006BA300, (void* self, void* edx, uint32_t* a3));

// Vtable referenced by the original's call site (no store happens in this function).
[[maybe_unused]] static const void* s_structVtableRef = CSF_VTBL(0x006B9580);

static C_0082d410* __fastcall GetOrCreateStringEntry(S_006b9580* self, void* /*edx*/, C_0082d410* a2)
{
    int index = sub_6B9580(self, nullptr, reinterpret_cast<uint32_t*>(a2));
    if (index == -1)
    {
        // New entry: 28-byte block from the game heap.
        uint8_t* newEntry = static_cast<uint8_t*>(Mem_Alloc(28));
        C_0082d410* entry = nullptr;
        if (newEntry != nullptr)
        {
            CSF_AT(uint32_t, newEntry, 0x18) = 15;
            CSF_AT(uint32_t, newEntry, 0x14) = 0;
            CSF_AT(uint8_t, newEntry, 0x04) = 0;
            entry = reinterpret_cast<C_0082d410*>(newEntry);
        }

        AssignStringSubstring(entry, nullptr, a2, 0, 0xFFFFFFFFu);

        // Register the new entry in the table: {entry, 1}.
        alignas(8) uint32_t entryRef[2];
        entryRef[0] = reinterpret_cast<uint32_t>(entry);
        entryRef[1] = 1;
        sub_6BA300(self, nullptr, entryRef);
        return entry;
    }

    // Existing entry: bump its reference count and return it.
    uint8_t* table = reinterpret_cast<uint8_t*>(self->field_0004);
    int entryOffset = index * 8;
    int refCount = *reinterpret_cast<int*>(table + entryOffset + 4);
    *reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(self->field_0004) + entryOffset + 4) = refCount + 1;
    return *reinterpret_cast<C_0082d410**>(reinterpret_cast<uint8_t*>(self->field_0004) + entryOffset);
}

HOOK(0x006BA370, GetOrCreateStringEntry, "auto");
