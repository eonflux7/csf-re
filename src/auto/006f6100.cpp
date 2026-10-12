#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, ViewWindow_ResolveParam, 0x006DAFB0, (void* self, void* edx, int a2, int arg4, int a3));
CSF_FUNC(void, __fastcall, Clip_ClearLights, 0x006D2390, (void* self, void* edx));

static char __fastcall C_00862874_GetProjectionReadyFlag(C_00862874* self, void* /*edx*/)
{
    (void)CSF_VTBL(0x006D2390);

    // Cached result: already resolved, return it.
    if (self->field_0005)
        return static_cast<char>(self->field_0005);

    // Resolve the first view-window parameter from the 0x58 sub-object.
    char resolvedA = ViewWindow_ResolveParam(
        reinterpret_cast<void*>(&self->field_0058), nullptr,
        15,
        static_cast<int>(CSF_ADDR_DATA(0x008A80F8)),
        0);
    self->field_0005 = static_cast<uint8_t>(resolvedA);

    if (resolvedA)
    {
        // Resolve the second parameter from the 0x18 sub-object.
        char resolvedB = ViewWindow_ResolveParam(
            reinterpret_cast<void*>(&self->field_0018), nullptr,
            15,
            static_cast<int>(CSF_ADDR_DATA(0x008A80F4)),
            0);
        self->field_0005 = static_cast<uint8_t>(resolvedB);

        if (!resolvedB)
            Clip_ClearLights(reinterpret_cast<void*>(&self->field_0058), nullptr);
    }

    return static_cast<char>(self->field_0005);
}

HOOK(0x006F6100, C_00862874_GetProjectionReadyFlag, "auto");
