#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __fastcall, SetSpawnObjectEnabled, 0x007A68F0, (void* self, void* edx, int a1));

static int __fastcall C_00868abc_EnableSpawnObject(C_00868abc* self, void* /*edx*/)
{
    if (self->field_0020)
        return SetSpawnObjectEnabled(reinterpret_cast<void*>(self->field_0020), nullptr, 1);
    return 0;
}

HOOK(0x00751F60, C_00868abc_EnableSpawnObject, "auto");
