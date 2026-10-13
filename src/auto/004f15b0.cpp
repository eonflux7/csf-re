#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_FUNC(uint8_t*, __fastcall, S_004f0cd0_RefreshGroups, 0x004F0F50, (void* self, void* edx));

static int __fastcall S_004f0cd0_GetActiveGroupIndex(
    S_004f0cd0* self, void* /*edx*/)
{
    if (self->field_0040 && self->field_003c < g_fZero)
    {
        self->field_003c = 0.5f;
        S_004f0cd0_RefreshGroups(self, nullptr);
    }

    if (reinterpret_cast<uintptr_t>(self) + 0x0C != 0 &&
        CSF_AT(uint32_t, self, 0x14) != 0)
        return 2;

    if (self->field_0040 && self->field_003c < g_fZero)
    {
        self->field_003c = 0.5f;
        S_004f0cd0_RefreshGroups(self, nullptr);
    }

    if (reinterpret_cast<uintptr_t>(self) + 0x1C != 0 &&
        CSF_AT(uint32_t, self, 0x24) != 0)
        return 1;

    if (self->field_0040 && self->field_003c < g_fZero)
    {
        self->field_003c = 0.5f;
        S_004f0cd0_RefreshGroups(self, nullptr);
    }

    if (reinterpret_cast<uintptr_t>(self) + 0x2C == 0 ||
        CSF_AT(uint32_t, self, 0x34) == 0)
        return -1;

    return 0;
}

HOOK(0x004F15B0, S_004f0cd0_GetActiveGroupIndex, "auto");
