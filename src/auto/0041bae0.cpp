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

CSF_FUNC(int, __fastcall, C_0082deb8_ReleaseChildBase, 0x00418430, (void* self, void* edx));

// Releases the wheel-controller object held at +0x30 (virtual slot 0 with arg 1, then clears
// the pointer), then tail-calls the base child-release routine on the same `this`, returning its result.
static int __fastcall C_0082e280_ReleaseWheelObjectThenCleanup(C_0082e280* self, void* /*edx*/)
{
    void** wheelObject = reinterpret_cast<void**>(self->field_0030);
    if (wheelObject)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, wheelObject, 0)[0])(wheelObject, nullptr, 1);
        self->field_0030 = 0;
    }
    return C_0082deb8_ReleaseChildBase(self, nullptr);
}

HOOK(0x0041BAE0, C_0082e280_ReleaseWheelObjectThenCleanup, "auto");
