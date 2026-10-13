#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Original calls (bound by address, original argument order and conventions).
CSF_FUNC(void*, __cdecl, AllocateGatheringStorage, 0x00647160, (std::uint32_t size, const char* sourceFile, int sourceLine));
CSF_FUNC(uint8_t*, __fastcall, C_00867890_InitDefaults, 0x00740810, (void* self, void*));
CSF_FUNC(int, __fastcall, NetControlDDL_InitBase, 0x0074EBC0, (void* self, void* edx));

static char* __cdecl NetControlDDL_AllocateAndInit()
{
    // Allocate 0xF4 bytes (244) with the original source tag and line 11.
    uint8_t* self = static_cast<uint8_t*>(
        AllocateGatheringStorage(0xF4, CSF_STR(0x00868524, ".\\PCGenerados\\NetControlDDL.cpp"), 11));
    if (!self)
        return nullptr;

    NetControlDDL_InitBase(self, nullptr);

    // Embedded sub-object at +0xD8 (216).
    C_00867890_InitDefaults(self + 0xD8, nullptr);

    // Vtable of the embedded sub-object at +0xD8.
    *reinterpret_cast<void***>(self + 0xD8) = CSF_VTBL(0x00868FC8);

    // Object's own vtable.
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00868F88);

    return reinterpret_cast<char*>(self);
}

HOOK(0x0074E630, NetControlDDL_AllocateAndInit, "auto");
