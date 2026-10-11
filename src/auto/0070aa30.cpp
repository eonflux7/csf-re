#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, WaterFx_Reset, 0x006FF710, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, RenderHandle_Release, 0x006D6E60, (void* self, void* edx, int a2));

// Resets water state, clears field_0030, and if field_0004 is set, releases its render handle
// (handle is the return of RenderHandle_FromObject, passed as `this` to RenderHandle_Release).
static int __fastcall C_00863600_ReleaseRenderHandle(C_00863600* self, void* /*edx*/)
{
    WaterFx_Reset(self, nullptr);

    int result = static_cast<int>(self->field_0004);
    self->field_0030 = 0;
    if (result != 0)
    {
        int globalInt = GetGlobalInt();
        int renderHandle = RenderHandle_FromObject(
            reinterpret_cast<void*>(static_cast<intptr_t>(globalInt)), nullptr);

        result = static_cast<int>(RenderHandle_Release(
            reinterpret_cast<void*>(static_cast<intptr_t>(renderHandle)),
            nullptr,
            static_cast<int>(self->field_0004)));
        self->field_0004 = 0;
    }
    return result;
}

HOOK(0x70AA30, C_00863600_ReleaseRenderHandle, "auto");
