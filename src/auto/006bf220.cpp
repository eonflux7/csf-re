#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwFrameRemoveChild, 0x00623750, (int a1));

// Detaches the RwFrame held at field_000c from its parent (when it has one),
// then forwards a2's slot-31 virtual call with this as the argument.
static int __fastcall C_00860700_DetachFrameAndNotify(C_00860700* self, void* /*edx*/, int a2)
{
    int result = (int)self->field_000c;
    if (CSF_AT(int, reinterpret_cast<void*>(result), 4)) {
        result = RwFrameRemoveChild(result);
    }
    if (a2) {
        using VSlot7CFn = int(__fastcall*)(void* self, void* edx, C_00860700* arg);
        auto fn = reinterpret_cast<VSlot7CFn>(CSF_AT(void**, reinterpret_cast<void*>(a2), 0)[0x7C / 4]);
        return fn(reinterpret_cast<void*>(a2), nullptr, self);
    }
    return result;
}

HOOK(0x006BF220, C_00860700_DetachFrameAndNotify, "auto");
