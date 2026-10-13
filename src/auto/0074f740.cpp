#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Original calls (bound by address, original argument order and conventions).
CSF_FUNC(void*, __cdecl, AllocateGatheringStorage, 0x00647160, (std::uint32_t size, const char* sourceFile, int sourceLine));
CSF_FUNC(uint8_t*, __fastcall, C_00867890_InitDefaults, 0x00740810, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, sub_74FE40, 0x0074FE40, (void* self, void* edx));

// Allocates and initializes a NetPostmanDDL object (216 bytes). The original tail-jumps
// into the middle of an inferred block at 0x7436C0, so that body is reproduced inline here.
static char* __cdecl NetPostmanDDL_AllocateAndInit()
{
    // Allocate 0xD8 bytes (216) with the original source tag and line 11.
    uint8_t* self = static_cast<uint8_t*>(
        AllocateGatheringStorage(0xD8, CSF_STR(0x008687A8, ".\\PCGenerados\\NetPostmanDDL.cpp"), 11));
    if (!self)
        return nullptr;

    sub_74FE40(self, nullptr);

    // Embedded sub-object at +0xB8.
    uint8_t* subObject = self + 0xB8;
    C_00867890_InitDefaults(subObject, nullptr);

    // Vtables.
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00867B08);
    *reinterpret_cast<void***>(subObject) = CSF_VTBL(0x00867AF8);

    CSF_AT(int32_t, self, 0xC4) = -1;

    // Clear fields that are only written when they are non-zero.
    if (CSF_AT(int32_t, self, 0x44) != 0)
        CSF_AT(int32_t, self, 0x44) = 0;
    if (CSF_AT(int32_t, self, 0x6C) != 0)
        CSF_AT(int32_t, self, 0x6C) = 0;
    if (CSF_AT(int32_t, self, 0x50) != 0)
        CSF_AT(int32_t, self, 0x50) = 0;
    if (CSF_AT(int32_t, self, 0x78) != 0)
        CSF_AT(int32_t, self, 0x78) = 0;

    if (CSF_AT(int32_t, self, 0xA0) != 0 || CSF_AT(int32_t, self, 0xA4) != 0) {
        CSF_AT(int32_t, self, 0xA0) = 0;
        CSF_AT(int32_t, self, 0xA4) = 0;
    }

    if (CSF_AT(int32_t, self, 0x88) != 0 || CSF_AT(int32_t, self, 0x8C) != 0) {
        CSF_AT(int32_t, self, 0x88) = 0;
        CSF_AT(int32_t, self, 0x8C) = 0;
    }

    CSF_AT(int32_t, self, 0xD4) = 0;

    return reinterpret_cast<char*>(self);
}

HOOK(0x0074F740, NetPostmanDDL_AllocateAndInit, "auto");
