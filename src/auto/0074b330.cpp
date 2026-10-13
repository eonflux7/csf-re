#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Original calls (bound by address, original argument order and conventions).
CSF_FUNC(void*, __cdecl, AllocateGatheringStorage, 0x00647160, (std::uint32_t size, const char* sourceFile, int sourceLine));
CSF_FUNC(int, __fastcall, NetGhostBase_InitBase, 0x0074B940, (void* self, void* edx));
CSF_FUNC(uint8_t*, __fastcall, C_00867890_InitDefaults, 0x00740810, (void* self, void* edx));

static char* __cdecl NetGhostDDL_AllocateAndInit()
{
    // Allocate 0xA0 bytes (160) with the original source tag and line 11.
    uint8_t* self = static_cast<uint8_t*>(
        AllocateGatheringStorage(0xA0, CSF_STR(0x008680F8, ".\\PCGenerados\\NetGhostDDL.cpp"), 11));
    if (!self)
        return nullptr;

    NetGhostBase_InitBase(self, nullptr);

    uint8_t* sub80 = self + 0x80;
    C_00867890_InitDefaults(sub80, nullptr);

    // Vtables: object's own at +0x0, embedded sub-object at +0x80.
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008675C8);
    *reinterpret_cast<void***>(sub80) = CSF_VTBL(0x008675B8);

    CSF_AT(int32_t, self, 0x8C) = -1;

    if (CSF_AT(int32_t, self, 0x5C) != -1)
        CSF_AT(int32_t, self, 0x5C) = -1;

    if (CSF_AT(uint8_t, self, 0x4A))
        CSF_AT(uint8_t, self, 0x4A) = 0;

    if (CSF_AT(uint8_t, self, 0x44))
        CSF_AT(uint8_t, self, 0x44) = 0;

    if (CSF_AT(int32_t, self, 0x68) != -1 || CSF_AT(int32_t, self, 0x6C) != -1) {
        CSF_AT(int32_t, self, 0x68) = -1;
        CSF_AT(int32_t, self, 0x6C) = -1;
    }

    if (CSF_AT(uint8_t, self, 0x47))
        CSF_AT(uint8_t, self, 0x47) = 0;

    CSF_AT(int32_t, self, 0x9C) = -1082130432; // float -1.0f

    return reinterpret_cast<char*>(self);
}

HOOK(0x0074B330, NetGhostDDL_AllocateAndInit, "auto");
