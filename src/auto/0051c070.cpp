#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(uint32_t, __cdecl, MpSpawnPoints_RegisterObject, 0x475850, (int a1));

static uint32_t __fastcall C_00840e68_RegisterObject(C_00840e68* self, void* /*edx*/)
{
    uint32_t result = MpSpawnPoints_RegisterObject(reinterpret_cast<int>(self));
    self->field_0004 = 1;
    return result;
}

HOOK(0x51C070, C_00840e68_RegisterObject, "auto");
