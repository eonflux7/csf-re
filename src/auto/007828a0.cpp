#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x004109E0: __fastcall (self, edx, source, position, count), ret 12
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// __thiscall(this, a2), ret 4. Resets this object's header fields, assigns the string
// from a2 (substring 0..end), then copies four 0x50-byte blocks from a2 into this.
static C_0082d410* __fastcall CopyStateFromSource(C_0082d410* self, void* /*edx*/, C_0082d410* source)
{
    CSF_AT(std::uint32_t, self, 0x18) = 15;
    CSF_AT(std::uint32_t, self, 0x14) = 0;
    CSF_AT(std::uint8_t, self, 0x04) = 0;

    AssignStringSubstring(self, nullptr, source, 0, 0xFFFFFFFFu);

    std::memcpy(reinterpret_cast<std::uint8_t*>(self) + 0x1C,
                reinterpret_cast<const std::uint8_t*>(source) + 0x1C, 0x50);
    std::memcpy(reinterpret_cast<std::uint8_t*>(self) + 0x6C,
                reinterpret_cast<const std::uint8_t*>(source) + 0x6C, 0x50);
    std::memcpy(reinterpret_cast<std::uint8_t*>(self) + 0xBC,
                reinterpret_cast<const std::uint8_t*>(source) + 0xBC, 0x50);
    std::memcpy(reinterpret_cast<std::uint8_t*>(self) + 0x10C,
                reinterpret_cast<const std::uint8_t*>(source) + 0x10C, 0x50);

    return self;
}

HOOK(0x7828A0, CopyStateFromSource, "auto");
