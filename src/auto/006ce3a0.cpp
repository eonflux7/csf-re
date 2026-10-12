#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies a collision shape record from `source` into `self`: an identifier dword, two embedded
// strings, a block of 9 dwords at +0x3C..+0x5C, and the two dwords at +0x54/+0x58 plus +0x5C.
// Returns `self`.
static char* __fastcall CollisionShape_CopyFrom(char* self, void* /*edx*/, std::uint32_t* source)
{
    CSF_AT(std::uint32_t, self, 0x00) = CSF_AT(std::uint32_t, source, 0x00);

    AssignStringSubstring(self + 0x04, nullptr,
                          reinterpret_cast<const void*>(reinterpret_cast<const std::uint8_t*>(source) + 0x04),
                          0, 0xFFFFFFFFu);
    AssignStringSubstring(self + 0x20, nullptr,
                          reinterpret_cast<const void*>(reinterpret_cast<const std::uint8_t*>(source) + 0x20),
                          0, 0xFFFFFFFFu);

    CSF_AT(std::uint32_t, self, 0x3C) = CSF_AT(std::uint32_t, source, 0x3C);
    CSF_AT(std::uint32_t, self, 0x40) = CSF_AT(std::uint32_t, source, 0x40);
    CSF_AT(std::uint32_t, self, 0x44) = CSF_AT(std::uint32_t, source, 0x44);
    CSF_AT(std::uint32_t, self, 0x48) = CSF_AT(std::uint32_t, source, 0x48);
    CSF_AT(std::uint32_t, self, 0x4C) = CSF_AT(std::uint32_t, source, 0x4C);
    CSF_AT(std::uint32_t, self, 0x50) = CSF_AT(std::uint32_t, source, 0x50);
    CSF_AT(std::uint32_t, self, 0x54) = CSF_AT(std::uint32_t, source, 0x54);
    CSF_AT(std::uint32_t, self, 0x58) = CSF_AT(std::uint32_t, source, 0x58);
    CSF_AT(std::uint32_t, self, 0x5C) = CSF_AT(std::uint32_t, source, 0x5C);

    return self;
}

HOOK(0x6CE3A0, CollisionShape_CopyFrom, "auto");
