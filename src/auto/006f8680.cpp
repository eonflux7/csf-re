#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee: string assignment (substring copy) into the embedded string member.
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies one screen-effect entry from a2 into this, resetting the embedded name string first.
static std::uint32_t* __fastcall GfxScreenEffects_ProcessEntry(std::uint32_t* self, void* /*edx*/, std::uint32_t* a2)
{
    CSF_AT(std::uint32_t, self, 0x00) = a2[0];

    // Embedded string member at this+4: reset its state, then assign from a2+4.
    void* stringMember = reinterpret_cast<std::uint8_t*>(self) + 4;
    CSF_AT(std::uint32_t, self, 0x18) = 15;
    CSF_AT(std::uint32_t, self, 0x14) = 0;
    CSF_AT(std::uint8_t, self, 0x04) = 0;
    AssignStringSubstring(stringMember, nullptr, reinterpret_cast<std::uint8_t*>(a2) + 4, 0, static_cast<std::uint32_t>(-1));

    CSF_AT(std::uint32_t, self, 0x20) = CSF_AT(std::uint32_t, a2, 0x20);
    CSF_AT(std::uint32_t, self, 0x24) = CSF_AT(std::uint32_t, a2, 0x24);
    CSF_AT(std::uint32_t, self, 0x28) = CSF_AT(std::uint32_t, a2, 0x28);
    return self;
}

HOOK(0x006F8680, GfxScreenEffects_ProcessEntry, "auto");
