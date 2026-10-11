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

// Constructor for C_0086f128: stores the vtable and the int argument (+0x4),
// allocates a 16-byte block (zeroing dwords +0x4..+0xC) and keeps it at +0x8.
// Returns this (eax), callee pops 4 bytes of stack arguments.
static void* __fastcall C_0086f128_Construct(void* self, void* /*edx*/, int initialValue)
{
    CSF_AT(void*, self, 0x0) = (void*)CSF_VTBL(0x0086F128);
    CSF_AT(int, self, 0x4) = initialValue;

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

HOOK(0x762ED0, C_0086f128_Construct, "auto");
