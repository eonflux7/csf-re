#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_ObjectPropertyTolerance, 0x008603A0);

static int __fastcall GfxObjectProps_IsDefault(void* self, void* /*edx*/)
{
    if (CSF_AT(uint8_t, self, 0x0C) != 0 ||
        CSF_AT(uint32_t, self, 0x10) != 32 ||
        CSF_AT(uint32_t, self, 0x14) != 0 ||
        CSF_AT(uint32_t, self, 0x18) != 0 ||
        CSF_AT(uint8_t, self, 0x0D) != 2)
        return 0;

    if (!(std::fabs(CSF_AT(float, self, 0x1C)) <= g_ObjectPropertyTolerance) ||
        !(std::fabs(CSF_AT(float, self, 0x20)) <= g_ObjectPropertyTolerance) ||
        !(std::fabs(CSF_AT(float, self, 0x24)) <= g_ObjectPropertyTolerance))
        return 0;

    void* firstProperty = CSF_AT(void*, self, 0x00);
    if (firstProperty && CSF_AT(uint32_t, firstProperty, 0x14) != 0)
        return 0;

    void* secondProperty = CSF_AT(void*, self, 0x04);
    if (secondProperty && CSF_AT(uint32_t, secondProperty, 0x14) != 0)
        return 0;

    if (!(std::fabs(CSF_AT(float, self, 0x28)) <= g_ObjectPropertyTolerance) ||
        !(std::fabs(CSF_AT(float, self, 0x2C)) <= g_ObjectPropertyTolerance))
        return 0;

    return 1;
}

HOOK(0x006B8A30, GfxObjectProps_IsDefault, "auto");
