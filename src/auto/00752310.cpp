#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static char __fastcall C_00868b04_ResetSpawnMessage(C_00868b04* self, void* /*edx*/)
{
    void* object = reinterpret_cast<void*>(self->field_001c);
    if (object)
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[2])(object, nullptr);

    self->field_0004 = 0;
    return 1;
}

HOOK(0x00752310, C_00868b04_ResetSpawnMessage, "auto");
