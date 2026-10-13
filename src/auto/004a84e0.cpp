#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, CharTask_InitBase, 0x004DB4C0, (void* self, void* edx));

static uint32_t* __fastcall C_00834fa8_ConstructMountTaskComposite(
    C_00834fa8* self, void* /*edx*/, int argument)
{
    auto* bytes = reinterpret_cast<uint8_t*>(self);

    CharTask_InitBase(self, nullptr);
    self->vtbl = CSF_VTBL(0x00834FA8);
    CSF_AT(void*, self, 0x04) = CSF_VTBL(0x00834F8C);

    CharTask_InitBase(bytes + 0x14, nullptr);
    CSF_AT(void*, self, 0x14) = CSF_VTBL(0x00834838);
    CSF_AT(void*, self, 0x18) = CSF_VTBL(0x0083481C);

    CharTask_InitBase(bytes + 0x28, nullptr);
    CSF_AT(void*, self, 0x28) = CSF_VTBL(0x008348F0);
    CSF_AT(void*, self, 0x2C) = CSF_VTBL(0x008348D0);

    CharTask_InitBase(bytes + 0x40, nullptr);
    CSF_AT(void*, self, 0x40) = CSF_VTBL(0x00834E28);
    CSF_AT(void*, self, 0x44) = CSF_VTBL(0x00834E08);
    CSF_AT(uint32_t, self, 0x54) = 0;
    CSF_AT(uint32_t, self, 0x58) = 0;
    CSF_AT(uint32_t, self, 0x5C) = 0;

    CharTask_InitBase(bytes + 0x64, nullptr);
    CSF_AT(void*, self, 0x64) = CSF_VTBL(0x008349A8);
    CSF_AT(void*, self, 0x68) = CSF_VTBL(0x00834988);

    CharTask_InitBase(bytes + 0x78, nullptr);
    CSF_AT(void*, self, 0x78) = CSF_VTBL(0x00834EE8);
    CSF_AT(void*, self, 0x7C) = CSF_VTBL(0x00834EC8);
    CSF_AT(uint32_t, self, 0x8C) = 0;
    CSF_AT(uint32_t, self, 0x90) = 0;
    CSF_AT(uint32_t, self, 0x94) = 0;

    CharTask_InitBase(bytes + 0x9C, nullptr);
    CSF_AT(void*, self, 0x9C) = CSF_VTBL(0x00834A68);
    CSF_AT(void*, self, 0xA0) = CSF_VTBL(0x00834A4C);

    CharTask_InitBase(bytes + 0xB0, nullptr);
    CSF_AT(void*, self, 0xB0) = CSF_VTBL(0x00834B28);
    CSF_AT(void*, self, 0xB4) = CSF_VTBL(0x00834B08);

    CharTask_InitBase(bytes + 0xC4, nullptr);
    CSF_AT(void*, self, 0xC4) = CSF_VTBL(0x00834BE8);
    CSF_AT(void*, self, 0xC8) = CSF_VTBL(0x00834BCC);

    CharTask_InitBase(bytes + 0xD8, nullptr);
    CSF_AT(void*, self, 0xD8) = CSF_VTBL(0x00834CA8);
    CSF_AT(void*, self, 0xDC) = CSF_VTBL(0x00834C8C);

    CharTask_InitBase(bytes + 0xEC, nullptr);
    CSF_AT(void*, self, 0xEC) = CSF_VTBL(0x00834D68);
    CSF_AT(void*, self, 0xF0) = CSF_VTBL(0x00834D48);

    CSF_AT(int32_t, self, 0x104) = -1;
    CSF_AT(int32_t, self, 0x108) = -1;
    CSF_AT(void*, self, 0x100) = CSF_VTBL(0x0082D448);
    CSF_AT(int, self, 0x118) = argument;
    CSF_AT(uint32_t, self, 0x08) = reinterpret_cast<uint32_t>(self);
    CSF_AT(uint32_t, self, 0x0C) = reinterpret_cast<uint32_t>(self);

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004A84E0, C_00834fa8_ConstructMountTaskComposite, "auto");
