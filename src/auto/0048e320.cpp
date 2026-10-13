#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: GetValue8B0554 (__cdecl, no args) returns a pointer-sized value that holds the subsystem table.
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

static int __fastcall SceneObj_GetSurfaceId(C_00833b88* self, void* /*edx*/)
{
    int result = -1;

    // Object at self->field_002c; its dword at +0xD4 is the surface id source.
    uint8_t* surfaceOwner = reinterpret_cast<uint8_t*>(self->field_002c);
    if (static_cast<int>(CSF_AT(uint32_t, surfaceOwner, 0xD4)) != -1)
    {
        uint8_t* holder = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetValue8B0554()));
        uint8_t* subsystem = reinterpret_cast<uint8_t*>(CSF_AT(uintptr_t, holder, 0x1FD4));

        // Re-read the surface id after the call, as the original does.
        surfaceOwner = reinterpret_cast<uint8_t*>(self->field_002c);
        int surfaceId = static_cast<int>(CSF_AT(uint32_t, surfaceOwner, 0xD4));

        // vtable slot 0x10 / 4 = 4
        auto slotFn = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, subsystem, 0)[4]);
        return slotFn(subsystem, nullptr, surfaceId);
    }
    return result;
}

HOOK(0x0048E320, SceneObj_GetSurfaceId, "auto");
