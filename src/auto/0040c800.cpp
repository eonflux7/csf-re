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

CSF_FUNC(int, __fastcall, C_0082deb8_ReleaseChildBase_41E110, 0x0041E110, (C_0082e520* self, void* edx));

// Releases the object held at +0x8C (virtual slot 0 with arg 1, then clears the pointer),
// then tail-calls the base release routine on the same `this`, returning its result.
static int __fastcall C_0082db60_ReleaseHeldObjectThenChildBase(C_0082db60* self, void* /*edx*/)
{
    void** heldObject = reinterpret_cast<void**>(self->field_008c);
    if (heldObject)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, heldObject, 0)[0])(heldObject, nullptr, 1);
        self->field_008c = 0;
    }
    return C_0082deb8_ReleaseChildBase_41E110(static_cast<C_0082e520*>(self), nullptr);
}

HOOK(0x0040C800, C_0082db60_ReleaseHeldObjectThenChildBase, "auto");
