#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copy-constructs a C_0082d410 from another instance (a2 points at the source object).
// Copies the vtable word, two embedded string objects (at +0x04 and +0x30) and the
// dwords at +0x20..+0x2C and +0x4C.
static C_0082d410* __fastcall C_0082d410_CopyConstruct(C_0082d410* self, void* /*edx*/, C_0082d410** a2)
{
    C_0082d410* src = reinterpret_cast<C_0082d410*>(a2);

    CSF_AT(std::uint32_t, self, 0x00) = CSF_AT(std::uint32_t, src, 0x00);

    // First embedded string at self+0x04
    CSF_AT(std::uint32_t, self, 0x1C) = 15;
    CSF_AT(std::uint32_t, self, 0x18) = 0;
    CSF_AT(std::uint8_t, self, 0x08) = 0;
    AssignStringSubstring(reinterpret_cast<std::uint8_t*>(self) + 0x04, nullptr,
                          reinterpret_cast<const std::uint8_t*>(src) + 0x04, 0, 0xFFFFFFFFu);

    CSF_AT(std::uint32_t, self, 0x20) = CSF_AT(std::uint32_t, src, 0x20);
    CSF_AT(std::uint32_t, self, 0x24) = CSF_AT(std::uint32_t, src, 0x24);
    CSF_AT(std::uint32_t, self, 0x28) = CSF_AT(std::uint32_t, src, 0x28);
    CSF_AT(std::uint32_t, self, 0x2C) = CSF_AT(std::uint32_t, src, 0x2C);

    // Second embedded string at self+0x30
    CSF_AT(std::uint32_t, self, 0x48) = 15;
    CSF_AT(std::uint32_t, self, 0x44) = 0;
    CSF_AT(std::uint8_t, self, 0x34) = 0;
    AssignStringSubstring(reinterpret_cast<std::uint8_t*>(self) + 0x30, nullptr,
                          reinterpret_cast<const std::uint8_t*>(src) + 0x30, 0, 0xFFFFFFFFu);

    CSF_AT(std::uint32_t, self, 0x4C) = CSF_AT(std::uint32_t, src, 0x4C);

    return self;
}

HOOK(0x00500FE0, C_0082d410_CopyConstruct, "auto");
