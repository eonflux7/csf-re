#include <cstdint>
#include "globals.h"
#include "hook.h"

// Binds the unnamed callee at 0x743960 (__cdecl, one char argument).
CSF_FUNC(char, __cdecl, OptionsFile_ApplyFlagChange, 0x00743960, (char a1));

// __thiscall COptionsFile method: stores a flag byte at +0x10, then
// forwards its logical negation (a2 == 0) to the callee at 0x743960.
// The callee pops its own argument (cdecl), so this function's ret 4 covers only a2.
static int __fastcall COptionsFile_StoreFlagAndNotifyInverse(uint8_t* self, void* /*edx*/, char flag) {
    CSF_AT(char, self, 0x10) = flag;
    char inverted = static_cast<char>(flag == 0);
    return static_cast<int>(OptionsFile_ApplyFlagChange(inverted));
}

HOOK(0x0050B200, COptionsFile_StoreFlagAndNotifyInverse, "auto");
