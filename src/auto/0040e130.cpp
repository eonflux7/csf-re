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

CSF_FUNC(int, __fastcall, C_0082deb8_ReleaseChildBase_41E110, 0x0041E110, (C_0082e520* self, void*));

// Releases the child object held at +0x30 (virtual slot 0 with arg 1, then clears the pointer),
// then tail-calls the child-base release routine on the same `this`, returning its result.
static int __fastcall C_0082dd78_ReleaseOwnedChildThenBase(C_0082dd78* self, void* /*edx*/)
{
    void** ownedChild = reinterpret_cast<void**>(self->field_0030);
    if (ownedChild)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, ownedChild, 0)[0])(ownedChild, nullptr, 1);
        self->field_0030 = 0;
    }
    return C_0082deb8_ReleaseChildBase_41E110(reinterpret_cast<C_0082e520*>(self), nullptr);
}

HOOK(0x0040E130, C_0082dd78_ReleaseOwnedChildThenBase, "auto");
