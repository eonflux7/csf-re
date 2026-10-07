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

HOOK(0x4DB5C0, GetLinkedObjectValue, "auto");
