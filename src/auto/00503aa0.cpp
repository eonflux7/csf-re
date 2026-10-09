#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static uint32_t* __fastcall CSceneLoaderBase_Construct(
    CSceneLoaderBase* self, void* /*edx*/)
{
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083F4BC));
    self->vtbl = reinterpret_cast<decltype(self->vtbl)>(CSF_VTBL(0x0083F578));
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083F574));

    CSF_AT(uint32_t, self, 0x30) = 0;
    CSF_AT(uint32_t, self, 0x34) = 0;
    CSF_AT(uint32_t, self, 0x38) = 0;
    CSF_AT(uint32_t, self, 0x3C) = 0;
    CSF_AT(uint32_t, self, 0x40) = 0;
    CSF_AT(uint32_t, self, 0x44) = 0;
    CSF_AT(uint32_t, self, 0x48) = 10;
    CSF_AT(uint32_t, self, 0x4C) = 0;
    CSF_AT(uint32_t, self, 0x50) = 0;
    CSF_AT(uint32_t, self, 0x54) = 0;

    CSF_AT(uint32_t, self, 0x74) = 0;
    CSF_AT(uint32_t, self, 0x78) = 15;
    CSF_AT(uint8_t, self, 0x64) = 0;

    CSF_AT(uint32_t, self, 0x94) = 15;
    CSF_AT(uint32_t, self, 0x90) = 0;
    CSF_AT(uint8_t, self, 0x80) = 0;

    CSF_AT(uint32_t, self, 0xB0) = 15;
    CSF_AT(uint32_t, self, 0xAC) = 0;
    CSF_AT(uint8_t, self, 0x9C) = 0;

    CSF_AT(uint32_t, self, 0xD0) = 7;
    CSF_AT(uint32_t, self, 0xCC) = 0;
    CSF_AT(uint16_t, self, 0xBC) = 0;

    CSF_AT(uint32_t, self, 0xD4) = 0;
    CSF_AT(uint32_t, self, 0xD8) = 0;
    CSF_AT(uint32_t, self, 0xDC) = 0;
    CSF_AT(uint32_t, self, 0xF0) = 0;
    CSF_AT(uint8_t, self, 0xF4) = 0;
    CSF_AT(uint8_t, self, 0xF5) = 0;
    CSF_AT(uint8_t, self, 0xF6) = 0;
    CSF_AT(uint8_t, self, 0xF7) = 0;
    CSF_AT(uint8_t, self, 0xF8) = 0;

    reinterpret_cast<uint8_t*>(&self->field_0008)[0] = 0;
    reinterpret_cast<uint8_t*>(&self->field_0008)[1] = 0;
    CSF_AT(uint8_t, self, 0x58) = 0;
    CSF_AT(int32_t, self, 0x14) = -1;
    CSF_AT(int32_t, self, 0x0C) = -1;
    CSF_AT(int32_t, self, 0x1C) = -1;
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint32_t, self, 0x28) = 0;
    CSF_AT(uint32_t, self, 0x5C) = 0;
    CSF_AT(uint8_t, self, 0xB4) = 0;
    CSF_AT(uint8_t, self, 0xB5) = 0;
    CSF_AT(uint8_t, self, 0xB6) = 0;
    CSF_AT(uint8_t, self, 0xE0) = 0;
    CSF_AT(uint8_t, self, 0xE1) = 0;
    CSF_AT(void*, self, 0xE8) = nullptr;
    CSF_AT(uint32_t, self, 0xEC) = 0;
    CSF_AT(uint32_t, self, 0xE4) = 0;
    CSF_AT(uint32_t, self, 0x100) = 0;
    CSF_AT(uint8_t, self, 0x104) = 0;
    CSF_AT(int32_t, self, 0x18) = -1;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00503AA0, CSceneLoaderBase_Construct, "auto");
