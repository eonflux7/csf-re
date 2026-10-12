#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, C_0082daf4_PrepareMatchedEntry, 0x0040B5B0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0082daf4_FinishMatchedEntry, 0x0040B690, (void* self, void* edx));

// Target 0x40B770 (__thiscall, ret 0). Prepares the matched entry, then tail-jumps
// into 0x40B690 with the same `this`, returning its result.
static int __fastcall C_0082daf4_ProcessMatchedEntry(void* self, void* /*edx*/)
{
    C_0082daf4_PrepareMatchedEntry(self, nullptr);
    return C_0082daf4_FinishMatchedEntry(self, nullptr);
}

HOOK(0x0040B770, C_0082daf4_ProcessMatchedEntry, "auto");
