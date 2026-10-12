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

CSF_FUNC(int, __fastcall, C_0082deb8_ReleaseChildBase, 0x0040D170, (void* self, void* edx));

// Releases the object held at +0x30 (virtual slot 0 with arg 1, then clears the pointer),
// then tail-calls the base release routine on the same `this`, returning its result.
static int __fastcall C_0082dcf0_ReleaseOwnedObjectThenChildBase(C_0082dcf0* self, void* /*edx*/)
{
    void** ownedObject = reinterpret_cast<void**>(self->field_0030);
    if (ownedObject)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, ownedObject, 0)[0])(ownedObject, nullptr, 1);
        self->field_0030 = 0;
    }
    return C_0082deb8_ReleaseChildBase(self, nullptr);
}

HOOK(0x0040D9C0, C_0082dcf0_ReleaseOwnedObjectThenChildBase, "auto");
