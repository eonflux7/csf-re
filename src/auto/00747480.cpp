#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Original calls (bound by address, original argument order and conventions).
CSF_FUNC(void*, __cdecl, AllocateGatheringStorage, 0x00647160, (std::uint32_t size, const char* sourceFile, int sourceLine));
CSF_FUNC(uint32_t*, __cdecl, NetCogibleDdl_AllocListSentinelNode_73FBA0, 0x0073FBA0, ());
CSF_FUNC(uint8_t*, __fastcall, C_00867890_InitDefaults, 0x00740810, (void* self, void* edx));
CSF_FUNC(int*, __fastcall, sub_747570, 0x00747570, (void* self, void* edx));

static char* NetProyectilDDL_AllocateAndInit()
{
    // Allocate 0x120 bytes (288) with the original source tag and line 11.
    uint8_t* self = static_cast<uint8_t*>(
        AllocateGatheringStorage(0x120, CSF_STR(0x00867ECC, ".\\PCGenerados\\NetProyectilDDL.cpp"), 11));
    if (!self)
        return nullptr;

    sub_747570(self, nullptr);

    // Embedded sub-object at +0x70.
    uint8_t* subObject70 = self + 0x70;
    C_00867890_InitDefaults(subObject70, nullptr);
    *reinterpret_cast<void***>(subObject70) = CSF_VTBL(0x008677B0);

    // Object's own vtable.
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008677C0);

    // Embedded list sub-object at +0x8C.
    uint8_t* listObject8C = self + 0x8C;
    CSF_AT(uint32_t*, listObject8C, 4) = NetCogibleDdl_AllocListSentinelNode_73FBA0();
    CSF_AT(uint32_t, listObject8C, 8) = 0;

    CSF_AT(uint8_t, self, 0x98) = 0;
    CSF_AT(uint8_t, self, 0x99) = 0;
    CSF_AT(int32_t, self, 0xA0) = 0;
    CSF_AT(int32_t, self, 0xA4) = 0;
    CSF_AT(int32_t, self, 0xA8) = 0;
    CSF_AT(int32_t, self, 0xAC) = 0;
    CSF_AT(int32_t, self, 0xB0) = 0;
    CSF_AT(int32_t, self, 0xB4) = 0;
    CSF_AT(int32_t, self, 0x100) = 0;
    CSF_AT(int32_t, self, 0x104) = 0;
    CSF_AT(uint8_t, self, 0xFC) = 0;
    CSF_AT(uint8_t, self, 0xF8) = 0;

    CSF_AT(int32_t, self, 0x7C) = -1;
    CSF_AT(int32_t, self, 0x9C) = -1;

    return reinterpret_cast<char*>(self);
}

HOOK(0x00747480, NetProyectilDDL_AllocateAndInit, "auto");
