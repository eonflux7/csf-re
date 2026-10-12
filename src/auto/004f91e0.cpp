#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __cdecl, MpSpawnPoints_RegisterObject, 0x00475850, (int a1));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static float* __fastcall C_0083f1d8_Initialize(C_0083f1d8* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0083F1D8);
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0.0f;

    self->field_0010 = CSF_AT(float, GetGlobalValue_8BC1E0(), 0x9D0);
    self->field_0014 = CSF_AT(float, GetGlobalValue_8BC1E0(), 0x9D4);
    self->field_0018 = CSF_AT(float, GetGlobalValue_8BC1E0(), 0x9DC);
    self->field_001c = CSF_AT(float, GetGlobalValue_8BC1E0(), 0x9D8);
    self->field_0020 = CSF_AT(float, GetGlobalValue_8BC1E0(), 0x9E0);
    const float value = CSF_AT(float, GetGlobalValue_8BC1E0(), 0x9E4);
    self->field_0024 = value;
    self->field_0028 = (self->field_0020 - value) / self->field_001c;
    self->field_002c = self->field_0024 / (self->field_001c - self->field_0018);

    MpSpawnPoints_RegisterObject(reinterpret_cast<int>(self));
    return reinterpret_cast<float*>(self);
}

HOOK(0x004F91E0, C_0083f1d8_Initialize, "auto");
