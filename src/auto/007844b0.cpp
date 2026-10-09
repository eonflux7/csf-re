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

// Game heap allocator (original calls 0x6910F0 with a single cdecl size argument).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Constructor for CWeaponDefTable: stores the vtable and owner, allocates a
// 16-byte record block (zeroing its last three dwords) and keeps it at +0x8.
// Returns this (eax), callee pops 4 bytes of stack arguments.
static void* __fastcall CWeaponDefTable_Construct(void* self, void* /*edx*/, int owner)
{
    CSF_AT(void*, self, 0x0) = (void*)CSF_VTBL(0x008728A0);
    CSF_AT(uint32_t, self, 0x4) = static_cast<uint32_t>(owner);

    void* block = Mem_Alloc(16);
    if (block) {
        CSF_AT(uint32_t, block, 0x4) = 0;
        CSF_AT(uint32_t, block, 0x8) = 0;
        CSF_AT(uint32_t, block, 0xC) = 0;
        CSF_AT(uint32_t, self, 0x8) = static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(block));
    } else {
        CSF_AT(uint32_t, self, 0x8) = 0;
    }
    return self;
}

HOOK(0x7844B0, CWeaponDefTable_Construct, "auto");
