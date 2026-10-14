#include <cstdint>
#include <cfloat>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(uint32_t, controlWord, 0x008C1000);
CSF_GLOBAL(uint32_t, previousControlWord, 0x008C1004);

// FXSAVE captures the raw x87 control word without inline assembly. Only
// its low word replaces the global: FNSTCW also preserves the high word.
// The CRT API uses abstract flags, not the raw x87 bit layout. _controlfp
// selects 24-bit precision and masks five exceptions. Restore the captured
// state with its modified raw CW to also mask denormals, preserve reserved
// CW bits, and leave MXCSR and the rest of the floating-point state alone.
// Unlike the original x87-only sequence, this implementation requires FXSR.
// Stringify the compiler feature (not a game string/address).
#define X87_STATE_FEATURE(feature) __attribute__((target(#feature)))
X87_STATE_FEATURE(fxsr)
static void __cdecl InitializeX87ControlWord()
{
    alignas(16) unsigned char state[512];
    __builtin_ia32_fxsave(state);
    uint16_t raw;
    std::memcpy(&raw, state, sizeof(raw));
    // The CRT query includes FWAIT, before either global is modified.
    _controlfp(0, 0);
    controlWord = (controlWord & 0xFFFF0000u) | raw;
    previousControlWord = controlWord;
    controlWord = (controlWord & 0xFFFFFCFFu) | 0x3Fu;
    _controlfp(0x2001Fu, 0x3001Fu);
    const uint16_t updated = static_cast<uint16_t>(controlWord);
    std::memcpy(state, &updated, sizeof(updated));
    __builtin_ia32_fxrstor(state);
}
HOOK(0x0082C404, InitializeX87ControlWord, "initfn");
#undef X87_STATE_FEATURE
