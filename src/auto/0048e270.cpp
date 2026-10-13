#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "core/memory.h"

// Slot byte offsets used by the original (slot = offset / 4):
//   [obj + 0x64] -> slot 25, [obj + 0x9C] -> slot 39

static int __fastcall C_00833908_InitStateFlag(C_00833908* self, void* /*edx*/, int a2)
{
    int result = self->vGetField016c();
    if (result)
    {
        int childRaw = self->vGetField016c();
        void* child = reinterpret_cast<void*>(childRaw);
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, child, 0)[25])(child, nullptr, a2);
    }

    uint32_t stateObjRaw = self->field_0044;
    if (stateObjRaw)
    {
        void* stateObj = reinterpret_cast<void*>(stateObjRaw);
        return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, stateObj, 0)[39])(stateObj, nullptr, a2);
    }
    return result;
}

HOOK(0x0048E270, C_00833908_InitStateFlag, "auto");
