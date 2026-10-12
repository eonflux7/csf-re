#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());
CSF_FUNC(uint32_t*, __fastcall, InitializeSpawnDimensions, 0x007AAC00,
         (void* self, void* edx, uint32_t* a2));

static uint32_t* __fastcall C_00868af0_InitializeSpawnBuffer(
    C_00868af0* self, void* /*edx*/, uint32_t* dimensions, int count)
{
    self->vtbl = CSF_VTBL(0x00868AF0);
    CSF_AT(uint32_t, self, 0x10) = 8000;
    CSF_AT(uint32_t, self, 0x18) = 8000;
    CSF_AT(void**, self, 0x0C) = CSF_VTBL(0x00868ADC);
    self->field_0014 = 1;
    self->field_001c = 16;
    self->field_0004 = 0;

    InitializeSpawnDimensions(reinterpret_cast<uint8_t*>(self) + 0x0C,
                              nullptr, dimensions);

    self->field_0020 = static_cast<uint32_t>(count);
    self->field_0024 = 0;

    alignas(8) uint8_t requestBytes[0x10];
    auto* request = reinterpret_cast<uint32_t*>(requestBytes);
    request[0] = (self->field_001c >> 3) * self->field_0014
               * CSF_AT(uint32_t, self, 0x10)
               * static_cast<uint32_t>(count);
    request[1] = self->field_0014;
    request[2] = CSF_AT(uint32_t, self, 0x18);
    request[3] = self->field_001c;

    void* allocator = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
    CSF_AT(uint32_t, self, 0x08) =
        reinterpret_cast<uint32_t(__fastcall*)(void*, void*, uint32_t*)>(
            CSF_AT(void**, allocator, 0)[0x94 / 4])(
                allocator, nullptr, request);

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x007521D0, C_00868af0_InitializeSpawnBuffer, "auto");
