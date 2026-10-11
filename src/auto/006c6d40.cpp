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

// Bindings for the original functions this one calls.
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(int, __fastcall, C_00860894_Init, 0x006C45F0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __fastcall, C_00860288_RegisterEntry, 0x006C68F0, (void* self, void* edx, int a1, int a2, void* a4));

// Allocates a 28-byte entry wrapping a1, appends it to this object's pointer array
// (growing it when full) and then forwards to the registration routine.
static int __fastcall C_00860288_AddEntryAndRegister(C_00860288* self, void* /*edx*/, int a1)
{
    // Entry allocation: 28 bytes via the game heap.
    C_00860178* entry = static_cast<C_00860178*>(Mem_Alloc(28));
    uint32_t entryValue = 0;
    if (entry) {
        C_00860178_Construct(entry, nullptr);
        entry->vtbl = CSF_VTBL(0x8608B0);
        C_00860894_Init(reinterpret_cast<uint8_t*>(entry) + 8, nullptr, -1, 0);
        CSF_AT(int, entry, 4) = a1;
        entryValue = reinterpret_cast<uint32_t>(entry);
    }

    // Grow the pointer array when count >= capacity (unsigned compare).
    uint32_t count = CSF_AT(uint32_t, self, 0x24);
    uint32_t capacity = self->field_0020;
    if (count >= capacity) {
        (void)CSF_VTBL(0x0041E5F0);
        S_0041e5f0* growView = reinterpret_cast<S_0041e5f0*>(reinterpret_cast<uint8_t*>(self) + 0x1C);
        S_0041e5f0_GrowArrayCapacity(growView, nullptr, static_cast<int>(CSF_AT(uint32_t, self, 0x28)));
    }

    // Append the entry at the current count, then bump the count.
    uint32_t index = CSF_AT(uint32_t, self, 0x24);
    uint32_t* slotArray = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x1C));
    uint32_t* slot = reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(slotArray) + 4 * index);
    CSF_AT(uint32_t, self, 0x24) = index + 1;
    *slot = entryValue;

    return C_00860288_RegisterEntry(self, nullptr, a1, static_cast<int>(entryValue), nullptr);
}

HOOK(0x006C6D40, C_00860288_AddEntryAndRegister, "auto");
