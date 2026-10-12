#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static char __fastcall C_00868af0_ResetSpawnMessage(C_00868af0* self, void* /*edx*/)
{
    void* object = CSF_AT(void*, self, 0x008);
    if (object)
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[3])(object, nullptr);

    self->field_0004 = 0;
    return 1;
}

HOOK(0x007520C0, C_00868af0_ResetSpawnMessage, "auto");
