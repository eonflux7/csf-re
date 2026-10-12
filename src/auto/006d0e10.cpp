#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static int __fastcall C_00860e68_SetActiveState(C_00860e68* self, void* /*edx*/, char isActive)
{
    int result = 0;

    if (isActive)
    {
        self->vSlot7C();

        CSF_AT(uint8_t, GetCachedObjectProvider(reinterpret_cast<void*>(self->field_002c), nullptr), 2) = 3;
        GetCachedObjectProvider(reinterpret_cast<void*>(self->field_002c), nullptr);

        CSF_AT(uint8_t, GetCachedObjectProvider(reinterpret_cast<void*>(self->field_0030), nullptr), 2) = 1;
        result = static_cast<int>(reinterpret_cast<uintptr_t>(
            GetCachedObjectProvider(reinterpret_cast<void*>(self->field_0030), nullptr)));

        self->field_0019 = static_cast<uint8_t>(isActive);
    }
    else
    {
        result = static_cast<int>(self->vSlot78());
        self->field_0019 = 0;
    }

    return result;
}

HOOK(0x006D0E10, C_00860e68_SetActiveState, "auto");
