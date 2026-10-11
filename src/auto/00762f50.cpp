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

// Callee: tears down the sub-object owned through the buffer handle (__fastcall, ecx = handle).
CSF_FUNC(int, __fastcall, DestroyOwnedSubobject, 0x00510FF0, (void* self, void* edx));
// Callee: game heap free (__cdecl, caller pops).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destructor-style cleanup for C_0086f128: restores the class vtable, then releases the
// owned buffer stored at +0x8 (if any) and clears the slot.
static int __fastcall ReleaseOwnedBuffer(C_0086f128* self, void* /*edx*/)
{
    // Vtable store happens first, as in the original.
    CSF_AT(void*, self, 0) = CSF_VTBL(0x0086F128);

    uint32_t ownedBuffer = CSF_AT(uint32_t, self, 0x8);
    int result = 0;
    if (ownedBuffer)
    {
        DestroyOwnedSubobject(reinterpret_cast<void*>(ownedBuffer), nullptr);
        result = Mem_Free(reinterpret_cast<void*>(ownedBuffer));
        CSF_AT(uint32_t, self, 0x8) = 0;
    }
    return result;
}

HOOK(0x762F50, ReleaseOwnedBuffer, "auto");
