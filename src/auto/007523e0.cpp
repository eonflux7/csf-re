#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());
CSF_FUNC(uint32_t*, __fastcall, InitializeSpawnDimensions, 0x007AAC00, (void* self, void* edx, uint32_t* a2));

static char* __fastcall C_00868b04_InitializeSpawnBuffer(
    C_00868b04* self, void* /*edx*/, int dimensions, int count)
{
    self->field_0004 = 0;
    *(void***)self = CSF_VTBL(0x00868B04);

    auto* dimensionsObject = reinterpret_cast<uint8_t*>(self) + 0x08;
    CSF_AT(uint32_t, self, 0x0C) = 8000;
    self->field_0014 = 8000;
    CSF_AT(uint32_t, self, 0x08) =
        reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00868ADC));
    CSF_AT(uint32_t, self, 0x10) = 1;
    CSF_AT(uint32_t, self, 0x18) = 16;
    self->field_0020 = 0;
    self->field_0030 = 0;
    self->field_002c = 0;
    self->field_0028 = 0;

    InitializeSpawnDimensions(
        dimensionsObject, nullptr, reinterpret_cast<uint32_t*>(dimensions));

    self->field_0024 = count;
    self->field_0004 = 0;

    uint32_t byteCount = CSF_AT(uint32_t, self, 0x18) >> 3;
    byteCount *= CSF_AT(uint32_t, self, 0x10);
    byteCount *= CSF_AT(uint32_t, self, 0x0C);
    byteCount *= static_cast<uint32_t>(count);

    alignas(8) uint8_t descriptorBytes[0x10];
    CSF_AT(uint32_t, descriptorBytes, 0x00) = byteCount;
    CSF_AT(uint32_t, descriptorBytes, 0x04) = CSF_AT(uint32_t, self, 0x10);
    CSF_AT(uint32_t, descriptorBytes, 0x08) = self->field_0014;
    CSF_AT(uint32_t, descriptorBytes, 0x0C) = CSF_AT(uint32_t, self, 0x18);

    void* manager = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
    self->field_001c =
        reinterpret_cast<uint32_t(__fastcall*)(void*, void*, uint32_t*)>(
            CSF_AT(void**, manager, 0)[0x98 / 4])(
                manager, nullptr, reinterpret_cast<uint32_t*>(descriptorBytes));

    return reinterpret_cast<char*>(self);
}

HOOK(0x007523E0, C_00868b04_InitializeSpawnBuffer, "auto");
