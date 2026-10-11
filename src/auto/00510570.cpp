#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, sub_5105B0, 0x005105B0, (int a1, int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Releases the block held in field_0004 (with its companion value field_0008
// passed to sub_5105B0 first), then clears the three slots.
static int __fastcall S_00756260_ReleaseOwnedBlock(S_00756260* self, void* /*edx*/) {
    int result = static_cast<int>(self->field_0004);
    if (result) {
        sub_5105B0(static_cast<int>(self->field_0004), static_cast<int>(self->field_0008));
        result = static_cast<int>(self->field_0004);
        if (result) {
            result = Mem_FreeDebug(reinterpret_cast<void*>(self->field_0004));
        }
    }
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    return result;
}

HOOK(0x510570, S_00756260_ReleaseOwnedBlock, "auto");
