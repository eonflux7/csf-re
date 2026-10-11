#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, ObjectBase_Initialize, 0x0064EBA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Network_InitializeObjectState, 0x00747EE0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, C_008310b8_GetSelf, 0x004B2980, (C_008310b8* self, void*));
CSF_FUNC(int, __fastcall, Registry_Attach, 0x0065E310, (void* self, void* edx, int a2));

CSF_GLOBAL(uint32_t, g_NetCogibleRegistryHandle, 0x008BC128);

static int* __fastcall C_00867800_InitObjectState(C_00867800* self, void* /*edx*/)
{
    ObjectBase_Initialize(self, nullptr);

    CSF_AT(void**, self, 0x00) = CSF_VTBL(0x00867800);

    Network_InitializeObjectState(reinterpret_cast<uint8_t*>(self) + 0x44, nullptr);

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(reinterpret_cast<uint8_t*>(self) + 0x6C), nullptr);
    CSF_AT(uint8_t, self, 0x6D) = 0;
    CSF_AT(uint8_t, self, 0x6C) = 0;
    CSF_AT(uint8_t, self, 0x6E) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(reinterpret_cast<uint8_t*>(self) + 0x6F), nullptr);
    CSF_AT(uint8_t, self, 0x70) = 0;
    CSF_AT(uint8_t, self, 0x6F) = 0;
    CSF_AT(uint8_t, self, 0x71) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(reinterpret_cast<uint8_t*>(self) + 0x72), nullptr);
    CSF_AT(uint8_t, self, 0x73) = 0;
    CSF_AT(uint8_t, self, 0x72) = 0;
    CSF_AT(uint8_t, self, 0x74) = 0;

    Registry_Attach(reinterpret_cast<uint8_t*>(self) + 0x2C, nullptr,
                    static_cast<int>(g_NetCogibleRegistryHandle));

    return reinterpret_cast<int*>(self);
}

HOOK(0x00742C10, C_00867800_InitObjectState, "auto");
