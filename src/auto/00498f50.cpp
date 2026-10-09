#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall CharActionFsm_SetCharacterFlag(
    void* self, void* /*edx*/, int flag, char enabled)
{
    if (flag == 32)
    {
        if (enabled)
        {
            CSF_AT(int32_t, self, 0x2B0) = CSF_AT(int32_t, self, 0x298);
            CSF_AT(int32_t, self, 0x2B8) = CSF_AT(int32_t, self, 0x2A0);
            CSF_AT(int32_t, self, 0x2BC) = CSF_AT(int32_t, self, 0x2A4);
            CSF_AT(int32_t, self, 0x2C0) = CSF_AT(int32_t, self, 0x2A8);
            CSF_AT(int32_t, self, 0x2C4) = CSF_AT(int32_t, self, 0x2AC);
        }
        else
        {
            CSF_AT(int32_t, self, 0x2B0) = 0;
            CSF_AT(int32_t, self, 0x2B8) = -1;
            CSF_AT(int32_t, self, 0x2BC) = -1;
            CSF_AT(int32_t, self, 0x2C0) = -1;
            CSF_AT(int32_t, self, 0x2C4) = -1;
        }
    }

    if (enabled)
    {
        CSF_AT(int32_t, self, 0x2E0) |= flag;
        return flag;
    }

    CSF_AT(int32_t, self, 0x2E0) &= ~flag;
    return ~flag;
}

HOOK(0x00498F50, CharActionFsm_SetCharacterFlag, "auto");
