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

CSF_FUNC(int, __fastcall, C_0082e520_BaseCleanup, 0x0041E110, (void* self, void* edx));

// Releases the object held at +0x10 (virtual slot 0 with arg 1, then clears the pointer),
// then tail-calls the base cleanup routine on the same `this`, returning its result.
static int __fastcall C_0082db10_ReleaseOwnedObjectThenCleanup(C_0082db10* self, void* /*edx*/)
{
    void** ownedObject = reinterpret_cast<void**>(self->field_0010);
    if (ownedObject)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, ownedObject, 0)[0])(ownedObject, nullptr, 1);
        self->field_0010 = 0;
    }
    return C_0082e520_BaseCleanup(self, nullptr);
}

HOOK(0x004116B0, C_0082db10_ReleaseOwnedObjectThenCleanup, "auto");
