#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(void*, __fastcall, C_00830438_Construct, 0x00712CC0, (void* self, void* edx));

static C_008314f8* __fastcall CMissionScript_Construct(C_008314f8* self, void* /*edx*/)
{
    CSF_AT(void*, self, 0x04) = CSF_VTBL(0x0082D428);
    CSF_AT(void*, self, 0x00) = CSF_VTBL(0x008314F8);
    CSF_AT(void*, self, 0x04) = CSF_VTBL(0x008314DC);

    CSF_AT(std::uint32_t, self, 0x0C) = 0;
    CSF_AT(std::uint32_t, self, 0x10) = 0;
    CSF_AT(std::uint32_t, self, 0x14) = 0;
    CSF_AT(std::uint32_t, self, 0x18) = 0;
    CSF_AT(std::uint32_t, self, 0x1C) = 0;
    CSF_AT(std::uint32_t, self, 0x20) = 0;
    CSF_AT(std::uint32_t, self, 0x24) = 10;
    CSF_AT(std::uint32_t, self, 0x28) = 0;
    CSF_AT(std::uint32_t, self, 0x2C) = 0;
    CSF_AT(std::uint32_t, self, 0x30) = 0;
    CSF_AT(std::uint32_t, self, 0x34) = 10;
    CSF_AT(std::uint32_t, self, 0x38) = 0;
    CSF_AT(std::uint32_t, self, 0x3C) = 0;
    CSF_AT(std::uint32_t, self, 0x44) = 0;
    CSF_AT(std::uint32_t, self, 0x48) = 0;
    CSF_AT(std::uint32_t, self, 0x4C) = 0;
    CSF_AT(std::uint32_t, self, 0x50) = 0;
    CSF_AT(std::uint32_t, self, 0x58) = 0;
    CSF_AT(std::uint32_t, self, 0x5C) = 0;
    CSF_AT(std::uint32_t, self, 0x60) = 0;
    CSF_AT(std::uint32_t, self, 0x68) = 0;
    CSF_AT(std::uint32_t, self, 0x6C) = 0;
    CSF_AT(std::uint32_t, self, 0x70) = 0;
    CSF_AT(std::uint32_t, self, 0x78) = 0;
    CSF_AT(std::uint32_t, self, 0x7C) = 0;
    CSF_AT(std::uint32_t, self, 0x80) = 0;
    CSF_AT(std::uint32_t, self, 0x84) = 0;
    CSF_AT(std::uint32_t, self, 0x88) = 0;
    CSF_AT(std::uint32_t, self, 0x8C) = 0;
    CSF_AT(std::uint32_t, self, 0x90) = 10;
    CSF_AT(void*, self, 0x94) = CSF_VTBL(0x008314C0);
    CSF_AT(std::uint32_t, self, 0x9C) = 0;
    CSF_AT(std::uint32_t, self, 0xA0) = 0;
    CSF_AT(std::uint32_t, self, 0xA4) = 0;
    CSF_AT(std::uint32_t, self, 0xB0) = 0;
    CSF_AT(std::uint32_t, self, 0xB4) = 0;
    CSF_AT(std::uint32_t, self, 0xB8) = 0;
    CSF_AT(std::uint32_t, self, 0xBC) = 10;
    CSF_AT(std::uint32_t, self, 0xC0) = 0;
    CSF_AT(std::uint32_t, self, 0xC4) = 0;
    CSF_AT(std::uint32_t, self, 0xC8) = 0;
    CSF_AT(std::uint32_t, self, 0xCC) = 10;
    CSF_AT(std::uint8_t, self, 0xAC) = 0;
    CSF_AT(std::uint8_t, self, 0xD0) = 0;

    void* auxiliaryObject = Mem_Alloc(16);
    if (auxiliaryObject)
    {
        C_00830438_Construct(auxiliaryObject, nullptr);
        CSF_AT(void*, auxiliaryObject, 0x0C) = CSF_VTBL(0x0082D428);
        CSF_AT(void*, auxiliaryObject, 0x00) = CSF_VTBL(0x00831470);
        CSF_AT(void*, auxiliaryObject, 0x0C) = CSF_VTBL(0x00831450);
        CSF_AT(void*, self, 0xA8) = auxiliaryObject;
    }
    else
    {
        CSF_AT(void*, self, 0xA8) = nullptr;
    }

    return self;
}

HOOK(0x0046ADA0, CMissionScript_Construct, "auto");
