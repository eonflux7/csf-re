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

// Destructor of the base C_0083dbb0 (same callee as in the packet's callee list).
CSF_FUNC(int, __fastcall, C_0083dbb0_Destroy_4D7A00, 0x004D7A00, (C_0083dbb0* self, void*));
// Game heap free, cdecl, one argument.
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
// Cleanup of the embedded C_0082d410 sub-object at +0x340 (unnamed in the packet; name chosen here).
CSF_FUNC(void, __fastcall, C_0082d410_Cleanup, 0x004D4920, (void* self, void* edx));

// Destructor body for C_0083c198. Takes one stack argument (flags byte; bit 0 = free storage).
static int __fastcall C_0083c198_Destroy(C_0083c198* self, void* /*edx*/, char flags)
{
    // Embedded sub-object at +0x340: its vtable is stored at +0x3BC before its cleanup runs.
    CSF_AT(void*, self, 0x3BC) = CSF_VTBL(0x0082D410);
    C_0082d410_Cleanup(reinterpret_cast<uint8_t*>(self) + 0x340, nullptr);

    C_0083dbb0_Destroy_4D7A00(self, nullptr);

    if ((flags & 1) != 0) {
        CoreMemory_Free(self);
    }

    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x004D5160, C_0083c198_Destroy, "auto");
