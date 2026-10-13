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

CSF_FUNC(unsigned int, __fastcall, C_00861d48_ReleaseOwnedEntries, 0x006E6C70, (void* self, void* edx));

// Releases the object held at +0x4C (virtual slot 0 with arg 1, then clears the pointer),
// then tail-calls the base release routine on the same `this`, returning its result.
static unsigned int __fastcall C_008632c0_ReleaseOwnedObjectThenBase(C_008632c0* self, void* /*edx*/)
{
    void** heldObject = reinterpret_cast<void**>(self->field_004c);
    if (heldObject)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, heldObject, 0)[0])(heldObject, nullptr, 1);
        self->field_004c = 0;
    }
    return C_00861d48_ReleaseOwnedEntries(static_cast<void*>(self), nullptr);
}

HOOK(0x007066E0, C_008632c0_ReleaseOwnedObjectThenBase, "auto");
