#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Original 0x0070BB10: __fastcall(self, edx, char a2), callee pops 4.
CSF_FUNC(uint32_t*, __fastcall, sub_70BB10, 0x0070BB10, (void* self, void* edx, char a2));

// Hook for 0x0070BB40: `sub ecx, 4; jmp 0x70BB10`.
// Adjusts `this` by -4 and forwards the single char argument to the callee,
// whose `ret 4` pops it, so this wrapper also pops 4 via callee-cleanup.
static uint32_t* __fastcall C_0082d410_ForwardAtMinusFour(void* self, void* /*edx*/, char a2) {
    void* adjusted = reinterpret_cast<uint8_t*>(self) - 4;
    return sub_70BB10(adjusted, nullptr, a2);
}

HOOK(0x70BB40, C_0082d410_ForwardAtMinusFour, "auto");
