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

// Binds to the original S_0050f230 insert-unique-key routine (__thiscall, ret 8).
CSF_FUNC(int, __fastcall, S_0050f230_InsertUniqueKey, 0x00519EC0,
         (S_0050f230* self, void* /*edx*/, int resultAddress, uint32_t* value));

// Binds to the original NetPerfCounters entry resolver (__thiscall, ret 4).
CSF_FUNC(int, __fastcall, NetPerfCounters_ResolveEntry, 0x0078BB40,
         (C_00872abc* self, void* /*edx*/, int entryIndex));

// Loads the element at entryIndex (164-byte stride) from the table referenced by
// this->field_0004, inserts it under a unique key, then resolves the entry.
static int __fastcall C_00840698_LoadElementAndRefresh(C_00840698* self, void* /*edx*/, int entryIndex)
{
    // Stack slot the callee writes its result into (original frame: [ebp-8], 8 bytes).
    alignas(8) uint8_t resultBytes[8];

    // Table descriptor referenced by field_0004; its +4 dword is the element array base.
    uint8_t* tableDescriptor = reinterpret_cast<uint8_t*>(self->field_0004);
    uint32_t elementArrayBase = CSF_AT(uint32_t, tableDescriptor, 4);
    uint32_t* element = reinterpret_cast<uint32_t*>(
        elementArrayBase + static_cast<uint32_t>(entryIndex) * 164u);

    // Original passes this + 8 (lea ecx, [esi+8]) as the S_0050f230 sub-object.
    S_0050f230* keyTable = reinterpret_cast<S_0050f230*>(reinterpret_cast<uint8_t*>(self) + 8);
    S_0050f230_InsertUniqueKey(keyTable, nullptr,
                               static_cast<int>(reinterpret_cast<uintptr_t>(resultBytes)),
                               element);

    return NetPerfCounters_ResolveEntry(reinterpret_cast<C_00872abc*>(self), nullptr, entryIndex);
}

HOOK(0x0051B5B0, C_00840698_LoadElementAndRefresh, "auto");
