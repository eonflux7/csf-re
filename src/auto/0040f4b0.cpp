#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Refresh routine called after a component flag changes (sub_40F150).
CSF_FUNC(char, __fastcall, SceneLoader_RefreshAfterFlagChange, 0x0040F150, (void* self, void* edx));

// Sets or clears the bits in `flagMask` of the component flag word at +0x14.
// Does nothing (returns the requested state) if the bits already match the request.
static char __fastcall SceneLoader_SetComponentFlag(C_0082ddf8* self, void* /*edx*/, int flagMask, char wantSet)
{
    char result = wantSet;

    uint32_t mask = static_cast<uint32_t>(flagMask);
    char isAlreadySet = ((self->field_0014 & mask) == mask) ? 1 : 0;

    if (isAlreadySet != wantSet)
    {
        uint32_t flags = self->field_0014;
        if (wantSet)
            self->field_0014 = mask | flags;
        else
            self->field_0014 = ~mask & flags;

        return SceneLoader_RefreshAfterFlagChange(self, nullptr);
    }

    return result;
}

HOOK(0x0040F4B0, SceneLoader_SetComponentFlag, "auto");
