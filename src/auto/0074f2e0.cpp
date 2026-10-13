#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Original calls (bound by address, original argument order and conventions).
CSF_FUNC(void*, __cdecl, AllocateGatheringStorage, 0x00647160, (std::uint32_t size, const char* sourceFile, int sourceLine));
CSF_FUNC(int, __fastcall, NetPlayerListDDL_InitBase, 0x0074F550, (void* self, void* edx));
CSF_FUNC(uint8_t*, __fastcall, C_00867890_InitDefaults, 0x00740810, (void* self, void* edx));

static char* __cdecl NetPlayerListDDL_AllocateAndInit()
{
    // Allocate 0x170 bytes (368) with the original source tag and line 11.
    uint8_t* self = static_cast<uint8_t*>(
        AllocateGatheringStorage(0x170, CSF_STR(0x00868698, ".\\PCGenerados\\NetPlayerListDDL.cpp"), 11));
    if (!self)
        return nullptr;

    NetPlayerListDDL_InitBase(self, nullptr);

    // Embedded sub-object at +0x154 (340).
    uint8_t* subObject154 = self + 0x154;
    C_00867890_InitDefaults(subObject154, nullptr);

    // Vtable of the embedded sub-object at +0x154.
    *reinterpret_cast<void***>(subObject154) = CSF_VTBL(0x00869018);

    // Object's own vtable.
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00868FD8);

    return reinterpret_cast<char*>(self);
}

HOOK(0x0074F2E0, NetPlayerListDDL_AllocateAndInit, "auto");

