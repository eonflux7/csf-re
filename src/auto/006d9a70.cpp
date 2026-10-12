#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee: resets one embedded particle sub-object (stdcall, one pointer arg, ret 4).
CSF_FUNC(int, __stdcall, PTank_ClearParticleSlot, 0x006D97D0, (int a1));

static int __fastcall PTank_ResetState(void* self, void* /*edx*/) {
    uint8_t* base = static_cast<uint8_t*>(self);

    // Clear the eight embedded sub-objects at +8, +24, +40, +56, +88, +104, +120, +136.
    PTank_ClearParticleSlot(static_cast<int>(reinterpret_cast<uintptr_t>(base + 8)));
    PTank_ClearParticleSlot(static_cast<int>(reinterpret_cast<uintptr_t>(base + 24)));
    PTank_ClearParticleSlot(static_cast<int>(reinterpret_cast<uintptr_t>(base + 40)));
    PTank_ClearParticleSlot(static_cast<int>(reinterpret_cast<uintptr_t>(base + 56)));
    PTank_ClearParticleSlot(static_cast<int>(reinterpret_cast<uintptr_t>(base + 88)));
    PTank_ClearParticleSlot(static_cast<int>(reinterpret_cast<uintptr_t>(base + 104)));
    PTank_ClearParticleSlot(static_cast<int>(reinterpret_cast<uintptr_t>(base + 120)));
    PTank_ClearParticleSlot(static_cast<int>(reinterpret_cast<uintptr_t>(base + 136)));

    CSF_AT(uint8_t, self, 0x99) = 1;
    CSF_AT(uint32_t, self, 0xE8) = 0;
    CSF_AT(uint32_t, self, 0xEC) = 0;
    CSF_AT(uint32_t, self, 0xF0) = 0;
    CSF_AT(uint32_t, self, 0xF4) = 0;
    CSF_AT(uint32_t, self, 0x130) = 0;
    CSF_AT(uint32_t, self, 0x134) = 0;
    CSF_AT(uint32_t, self, 0x138) = 0;

    return 0;
}

HOOK(0x006D9A70, PTank_ResetState, "auto");
