#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Original calls (bound by address, original argument order and conventions).
CSF_FUNC(void*, __cdecl, AllocateGatheringStorage, 0x00647160, (std::uint32_t size, const char* sourceFile, int sourceLine));
CSF_FUNC(uint32_t*, __cdecl, sub_73FBA0, 0x0073FBA0, ());
CSF_FUNC(uint8_t*, __fastcall, sub_740810, 0x00740810, (void* self, void* edx));
CSF_FUNC(int*, __fastcall, sub_742C10, 0x00742C10, (void* self, void* edx));

static char* __cdecl NetCogibleDDL_AllocateAndInit()
{
    // Allocate 0x128 bytes (296) with the original source tag and line 11.
    uint8_t* self = static_cast<uint8_t*>(
        AllocateGatheringStorage(0x128, CSF_STR(0x00867940, ".\\PCGenerados\\NetCogibleDDL.cpp"), 11));
    if (!self)
        return nullptr;

    sub_742C10(self, nullptr);

    uint8_t* sub120 = self + 0x78;
    sub_740810(sub120, nullptr);

    // Vtable of the embedded sub-object at +0x78.
    *reinterpret_cast<void***>(sub120) = CSF_VTBL(0x00867840);

    // Object's own vtable.
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00867850);

    uint8_t* sub94 = self + 0x94;
    CSF_AT(uint32_t*, sub94, 4) = sub_73FBA0();
    CSF_AT(uint32_t, sub94, 8) = 0;

    CSF_AT(int32_t, self, 0x84) = -1;
    CSF_AT(int32_t, self, 0xA4) = -1;
    CSF_AT(int32_t, self, 0xA8) = -1;

    CSF_AT(uint8_t, self, 0xA0) = 0;
    CSF_AT(int32_t, self, 0xC0) = 0;
    CSF_AT(int32_t, self, 0xC4) = 0;
    CSF_AT(int32_t, self, 0xC8) = 0;
    CSF_AT(int32_t, self, 0xCC) = 0;
    CSF_AT(uint8_t, self, 0xD0) = 0;
    CSF_AT(uint8_t, self, 0x114) = 0;
    CSF_AT(uint8_t, self, 0x118) = 0;
    CSF_AT(int32_t, self, 0x120) = 0;
    CSF_AT(int32_t, self, 0x124) = 0;
    CSF_AT(int32_t, self, 0xB4) = 0;
    CSF_AT(int32_t, self, 0xB8) = 0;
    CSF_AT(uint8_t, self, 0x6F) = 0;
    CSF_AT(uint8_t, self, 0x6C) = 0;
    CSF_AT(uint8_t, self, 0x72) = 0;
    CSF_AT(uint8_t, self, 0xBC) = 0;
    CSF_AT(uint8_t, self, 0xBD) = 0;

    CSF_AT(uint8_t, self, 0xAC) = 1;
    CSF_AT(uint8_t, self, 0xAD) = 1;
    CSF_AT(int32_t, self, 0xB0) = -1082130432; // float -1.0f

    return reinterpret_cast<char*>(self);
}

HOOK(0x00741FC0, NetCogibleDDL_AllocateAndInit, "auto");
