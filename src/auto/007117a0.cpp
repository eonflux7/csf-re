#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Virtual slot 9 (byte offset 0x24) of the child object. The original calls it
// __stdcall with (child, 1), so the callee pops 8 bytes.
static int __fastcall C_00863d18_ReleaseOwnedChild(C_00863d18* self, void* edx)
{
    int result = static_cast<int>(self->field_0008);
    if (result != 0) {
        // mov ecx, [child] (the vtable); the original never writes edx, so the entry edx is passed on.
        void** vtbl = CSF_AT(void**, result, 0);
        auto releaseChild = reinterpret_cast<int(__fastcall*)(void*, void*, int, int)>(vtbl[0x24 / 4]);
        return releaseChild(vtbl, edx, result, 1);
    }
    return result;
}

HOOK(0x7117A0, C_00863d18_ReleaseOwnedChild, "auto");
