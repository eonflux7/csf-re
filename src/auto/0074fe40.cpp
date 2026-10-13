#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_008310b8_GetSelf, 0x004B2980, (C_008310b8* self, void*));
CSF_FUNC(uint32_t*, __fastcall, ObjectBase_Initialize, 0x0064EBA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Registry_Attach, 0x0065E310, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_008310b8_ResetSpawnMessage, 0x00751AF0, (C_008310b8* self, void*));

CSF_GLOBAL(uint32_t, g_RequiredObjectClassId, 0x008BC184);

static uint32_t* __fastcall C_00867ab8_Initialize(C_00867ab8* self, void* /*edx*/)
{
    ObjectBase_Initialize(self, nullptr);
    self->vtbl = CSF_VTBL(0x00867AB8);

    auto* bytes = reinterpret_cast<uint8_t*>(self);

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x44), nullptr);
    CSF_AT(uint8_t, self, 0x48) = 0;
    CSF_AT(uint32_t, self, 0x44) = 0;
    CSF_AT(uint32_t, self, 0x4C) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x50), nullptr);
    CSF_AT(uint8_t, self, 0x54) = 0;
    CSF_AT(uint32_t, self, 0x50) = 0;
    CSF_AT(uint32_t, self, 0x58) = 0;

    C_008310b8_ResetSpawnMessage(reinterpret_cast<C_008310b8*>(bytes + 0x5C), nullptr);

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x6C), nullptr);
    CSF_AT(uint8_t, self, 0x70) = 0;
    CSF_AT(uint32_t, self, 0x6C) = 0;
    CSF_AT(uint32_t, self, 0x74) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x78), nullptr);
    CSF_AT(uint8_t, self, 0x7C) = 0;
    CSF_AT(uint32_t, self, 0x78) = 0;
    CSF_AT(uint32_t, self, 0x80) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x88), nullptr);
    CSF_AT(uint8_t, self, 0x90) = 0;
    CSF_AT(uint32_t, self, 0x88) = 0;
    CSF_AT(uint32_t, self, 0x8C) = 0;
    CSF_AT(uint32_t, self, 0x98) = 0;
    CSF_AT(uint32_t, self, 0x9C) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0xA0), nullptr);
    CSF_AT(uint8_t, self, 0xA8) = 0;
    CSF_AT(uint32_t, self, 0xA0) = 0;
    CSF_AT(uint32_t, self, 0xA4) = 0;
    CSF_AT(uint32_t, self, 0xB0) = 0;
    CSF_AT(uint32_t, self, 0xB4) = 0;

    Registry_Attach(bytes + 0x2C, nullptr, static_cast<int>(g_RequiredObjectClassId));
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0074FE40, C_00867ab8_Initialize, "auto");
