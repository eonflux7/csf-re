#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static bool __fastcall C_008316d8_CanReload(C_008316d8* self, void* /*edx*/)
{
    const auto* definition = reinterpret_cast<const uint8_t*>(self->field_0040);
    const int32_t reloadMode = CSF_AT(int32_t, definition, 0xD4);

    if (reloadMode == 1)
    {
        if (static_cast<int32_t>(self->field_0024) > 0 &&
            static_cast<int32_t>(self->field_001c) <
                CSF_AT(int32_t, definition, 0x104))
            return true;
    }
    else if (reloadMode == 2)
    {
        if (self->field_0020 &&
            static_cast<int32_t>(self->field_001c) <
                CSF_AT(int32_t, definition, 0x104))
            return true;
    }

    return false;
}

HOOK(0x004766F0, C_008316d8_CanReload, "auto");
