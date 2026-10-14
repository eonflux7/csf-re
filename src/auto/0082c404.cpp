#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(uint32_t, controlWord, 0x008C1000);
CSF_GLOBAL(uint32_t, previousControlWord, 0x008C1004);

// Static initialiser: 24-bit precision, all x87 exceptions masked. Assembly is reserved for thunks, so the raw
// x87 control word is read and written through FXSAVE/FXRSTOR (requires FXSR); MXCSR and the rest of the state are
// restored as captured. FNSTCW writes only the low word of the global; the high word keeps what it held.
#define X87_STATE_FEATURE(feature) __attribute__((target(#feature)))
X87_STATE_FEATURE(fxsr)
static void __cdecl InitializeX87ControlWord()
{
    alignas(16) unsigned char state[512];
    __builtin_ia32_fxsave(state);
    uint16_t raw;
    std::memcpy(&raw, state, sizeof(raw));
    controlWord = (controlWord & 0xFFFF0000u) | raw;
    previousControlWord = controlWord;
    controlWord = (controlWord & 0xFFFFFCFFu) | 0x3Fu;
    const uint16_t updated = static_cast<uint16_t>(controlWord);
    std::memcpy(state, &updated, sizeof(updated));
    __builtin_ia32_fxrstor(state);
}
HOOK(0x0082C404, InitializeX87ControlWord, "initfn");
#undef X87_STATE_FEATURE
