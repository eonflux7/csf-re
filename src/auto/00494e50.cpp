#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_008319c0_IsAttackOrReloadBusy, 0x00483380, (C_008319c0* self, void*));

static bool __fastcall IsActionAllowedNow(void* self, void* /*edx*/)
{
    bool result = true;
    if (!C_008319c0_IsAttackOrReloadBusy(CSF_AT(C_008319c0*, self, 0x44), nullptr))
    {
        if (!CSF_AT(uint8_t, self, 0x229))
            return false;
        int mode = CSF_AT(int, self, 0x2b0);
        if (mode != 2 && mode != 1)
            return false;
    }
    return result;
}

HOOK(0x00494E50, IsActionAllowedNow, "auto");
