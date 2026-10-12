#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Resets both embedded string members of C_0082d410 (at +0x00 and +0x1C) to empty
// and assigns the source string (a2 at +0x00, a2 + 0x1C at +0x1C) into each.
static int __fastcall C_0082d410_InitFromStrings(C_0082d410* self, void* /*edx*/, int a2)
{
    // First embedded string (at this + 0)
    CSF_AT(std::uint32_t, self, 0x18) = 15;
    CSF_AT(std::uint32_t, self, 0x14) = 0;
    *reinterpret_cast<std::uint8_t*>(&self->field_0004) = 0;
    AssignStringSubstring(self, nullptr, reinterpret_cast<const void*>(a2), 0, 0xFFFFFFFFu);

    // Second embedded string (at this + 0x1C)
    std::uint8_t* secondStr = reinterpret_cast<std::uint8_t*>(self) + 0x1C;
    CSF_AT(std::uint32_t, secondStr, 0x18) = 15;
    CSF_AT(std::uint32_t, secondStr, 0x14) = 0;
    CSF_AT(std::uint8_t, secondStr, 0x4) = 0;
    AssignStringSubstring(secondStr, nullptr,
                          reinterpret_cast<const void*>(a2 + 0x1C), 0, 0xFFFFFFFFu);

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(self));
}

HOOK(0x0043EC50, C_0082d410_InitFromStrings, "auto");
