#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S_00470ab0_ZeroAllFields, 0x00503570, (S_00470ab0* self, void*));
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(int, __fastcall, C_00860894_Init, 0x006C45F0, (void* self, void* edx, int a2, int a3));

static int __fastcall C_008611d4_Construct(C_008611d4* self, void* /*edx*/)
{
    C_00860178_Construct(reinterpret_cast<C_00860178*>(self), nullptr);
    CSF_AT(uint32_t, self, 0x04) =
        reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x0082D428));
    self->vtbl = CSF_VTBL(0x008611D4);
    CSF_AT(uint32_t, self, 0x04) =
        reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x008611B8));

    S_00470ab0_ZeroAllFields(
        reinterpret_cast<S_00470ab0*>(reinterpret_cast<uint8_t*>(self) + 0x08),
        nullptr);

    CSF_AT(uint32_t, self, 0xA8) = 0;
    CSF_AT(uint32_t, self, 0xAC) = 0;
    CSF_AT(uint32_t, self, 0xB0) = 0;
    CSF_AT(uint32_t, self, 0xB4) = 10;
    CSF_AT(uint32_t, self, 0xB8) = 0;
    CSF_AT(uint32_t, self, 0xBC) = 0;
    CSF_AT(uint32_t, self, 0xC0) = 0;
    CSF_AT(uint32_t, self, 0xC4) = 10;
    CSF_AT(uint32_t, self, 0xC8) = 0;
    CSF_AT(uint32_t, self, 0xCC) = 0;
    CSF_AT(uint32_t, self, 0xD0) = 0;
    CSF_AT(uint32_t, self, 0xD4) = 10;
    CSF_AT(uint32_t, self, 0xD8) = 0;
    CSF_AT(uint32_t, self, 0xDC) = 0;
    CSF_AT(uint32_t, self, 0xE0) = 0;
    CSF_AT(uint32_t, self, 0xE4) = 10;
    CSF_AT(uint32_t, self, 0xF8) = 0;
    CSF_AT(uint32_t, self, 0xFC) = 0;
    CSF_AT(uint32_t, self, 0x100) = 0;
    CSF_AT(uint32_t, self, 0x104) = 10;
    CSF_AT(uint32_t, self, 0x108) = 0;
    CSF_AT(uint32_t, self, 0x10C) = 0;
    CSF_AT(uint32_t, self, 0x110) = 0;
    CSF_AT(uint32_t, self, 0x114) = 10;

    C_00860894_Init(reinterpret_cast<uint8_t*>(self) + 0x11C, nullptr, -1, 0);

    CSF_AT(uint32_t, self, 0x130) = 0;
    CSF_AT(uint32_t, self, 0x134) = 0;
    CSF_AT(uint32_t, self, 0x138) = 0;
    CSF_AT(uint8_t, self, 0x98) = 0;
    CSF_AT(uint8_t, self, 0x99) = 0;
    CSF_AT(uint8_t, self, 0xA0) = 0;
    CSF_AT(uint32_t, self, 0xA4) = 0;
    CSF_AT(uint8_t, self, 0x118) = 0;
    CSF_AT(uint32_t, self, 0xE8) = 0;
    CSF_AT(uint32_t, self, 0xEC) = 0;
    CSF_AT(uint32_t, self, 0xF0) = 0;
    CSF_AT(uint32_t, self, 0xF4) = 0;
    CSF_AT(float, self, 0x9C) = 2000.0f;

    return reinterpret_cast<int>(self);
}

HOOK(0x006D9B10, C_008611d4_Construct, "auto");
