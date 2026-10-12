#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));

static C_0082d410* __fastcall C_0082d410_CopyConstruct(
    C_0082d410* self, void* /*edx*/, C_0082d410* source)
{
    CSF_AT(std::uint32_t, self, 0x18) = 15;
    CSF_AT(std::uint32_t, self, 0x14) = 0;
    *reinterpret_cast<std::uint8_t*>(&self->field_0004) = 0;
    AssignStringSubstring(self, nullptr, source, 0, -1);

    CSF_AT(std::uint32_t, self, 0x34) = 7;
    CSF_AT(std::uint32_t, self, 0x30) = 0;
    CSF_AT(std::uint16_t, self, 0x20) = 0;
    AssignStringRange_488830(
        reinterpret_cast<std::uint8_t*>(self) + 0x1C,
        nullptr,
        reinterpret_cast<std::uint8_t*>(source) + 0x1C,
        0, 0xFFFFFFFF);

    CSF_AT(std::uint32_t, self, 0x38) = CSF_AT(std::uint32_t, source, 0x38);
    CSF_AT(std::uint32_t, self, 0x3C) = CSF_AT(std::uint32_t, source, 0x3C);
    CSF_AT(std::uint32_t, self, 0x40) = CSF_AT(std::uint32_t, source, 0x40);
    CSF_AT(std::uint32_t, self, 0x44) = CSF_AT(std::uint32_t, source, 0x44);
    CSF_AT(std::uint32_t, self, 0x48) = CSF_AT(std::uint32_t, source, 0x48);
    CSF_AT(std::uint32_t, self, 0x4C) = CSF_AT(std::uint32_t, source, 0x4C);
    CSF_AT(std::uint32_t, self, 0x50) = CSF_AT(std::uint32_t, source, 0x50);
    CSF_AT(std::uint32_t, self, 0x54) = CSF_AT(std::uint32_t, source, 0x54);
    CSF_AT(std::uint32_t, self, 0x58) = CSF_AT(std::uint32_t, source, 0x58);
    CSF_AT(std::uint32_t, self, 0x5C) = CSF_AT(std::uint32_t, source, 0x5C);
    CSF_AT(std::uint32_t, self, 0x60) = CSF_AT(std::uint32_t, source, 0x60);
    CSF_AT(std::uint32_t, self, 0x64) = CSF_AT(std::uint32_t, source, 0x64);
    CSF_AT(std::uint32_t, self, 0x68) = CSF_AT(std::uint32_t, source, 0x68);
    CSF_AT(std::uint32_t, self, 0x6C) = CSF_AT(std::uint32_t, source, 0x6C);
    CSF_AT(std::uint32_t, self, 0x70) = CSF_AT(std::uint32_t, source, 0x70);
    CSF_AT(std::uint8_t, self, 0x74) = CSF_AT(std::uint8_t, source, 0x74);
    CSF_AT(std::uint32_t, self, 0x78) = CSF_AT(std::uint32_t, source, 0x78);
    return self;
}

HOOK(0x004F2410, C_0082d410_CopyConstruct, "auto");
