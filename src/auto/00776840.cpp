#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, ReleaseNativeHandle, 0x00775360, (int a1, int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static char __fastcall S_006d4160_ReleaseHandleAndClear(S_006d4160* self, void* /*edx*/)
{
    uint32_t handle = self->field_0004;
    uint32_t v2 = handle;
    if (v2 != 0)
    {
        ReleaseNativeHandle(static_cast<int>(v2), static_cast<int>(self->field_0008));
        v2 = self->field_0004;
        if (v2 != 0)
            v2 = static_cast<uint32_t>(Mem_FreeDebug(reinterpret_cast<void*>(self->field_0004)));
    }

    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    return static_cast<char>(v2);
}

HOOK(0x00776840, S_006d4160_ReleaseHandleAndClear, "auto");
