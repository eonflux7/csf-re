#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Original calls (bound by address, original argument order and conventions).
CSF_FUNC(void*, __cdecl, AllocateGatheringStorage, 0x00647160, (std::uint32_t size, const char* sourceFile, int sourceLine));
CSF_FUNC(uint8_t*, __fastcall, C_00867890_InitDefaults, 0x00740810, (void* self, void* edx));
CSF_FUNC(int, __fastcall, InitNetGameBase, 0x0074D180, (void* self, void* edx));

static char* __cdecl NetGameDDL_Create()
{
    // Allocate 0x368 (872) bytes with the original source tag and line 11.
    uint8_t* self = static_cast<uint8_t*>(
        AllocateGatheringStorage(0x368, CSF_STR(0x00868294, ".\\PCGenerados\\NetGameDDL.cpp"), 11));
    if (!self)
        return nullptr;

    InitNetGameBase(self, nullptr);

    // Embedded sub-object at +0x308.
    uint8_t* subDefaults = self + 0x308;
    C_00867890_InitDefaults(subDefaults, nullptr);

    // Vtables: the object's own vtable, then the embedded sub-object's vtable.
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00868010);
    *reinterpret_cast<void***>(subDefaults) = CSF_VTBL(0x00868000);

    CSF_AT(int32_t, self, 0x314) = -1;
    CSF_AT(uint8_t, self, 0x5C) = 1;
    CSF_AT(int32_t, self, 0x2E8) = 0;
    CSF_AT(int32_t, self, 0x2EC) = 0;
    CSF_AT(float, self, 0x32C) = 15.0f;
    CSF_AT(uint8_t, self, 0x300) = 0;
    CSF_AT(uint16_t, self, 0x2D6) = 0;
    CSF_AT(uint16_t, self, 0x2DC) = 0;
    CSF_AT(uint16_t, self, 0x2C4) = 0;
    CSF_AT(int32_t, self, 0x350) = 0;
    CSF_AT(int32_t, self, 0x354) = 0;
    CSF_AT(int32_t, self, 0x358) = 0;
    CSF_AT(int32_t, self, 0x35C) = 0;
    CSF_AT(uint8_t, self, 0x360) = 0;
    CSF_AT(uint8_t, self, 0x361) = 0;
    CSF_AT(uint8_t, self, 0x331) = 0;

    if (CSF_AT(int32_t, self, 0x2B8) != 1)
        CSF_AT(int32_t, self, 0x2B8) = 1;

    CSF_AT(uint8_t, self, 0x5F) = 0;
    CSF_AT(uint8_t, self, 0x332) = 0;

    return reinterpret_cast<char*>(self);
}

HOOK(0x0074C4C0, NetGameDDL_Create, "auto");
