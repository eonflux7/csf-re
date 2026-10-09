#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Returns the value stored at +0x10 of the object referenced by this+4,
// or 0 when that pointer is null.
static int __fastcall GetLinkedObjectValue(void* self, void* /*edx*/)
{
    void* linked = CSF_AT(void*, self, 0x4);
    if (linked)
        return CSF_AT(int, linked, 0x10);
    return 0;
}

// LTCG callers (0x4B0610, 0x4B3C10, 0x4B3D30) keep a value in ecx across the call: the original never writes ecx.
HOOK_KEEP_ECX(0x4DB5C0, GetLinkedObjectValue, 0, "auto");
