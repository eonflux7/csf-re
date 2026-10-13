#include <cstdint>
#include "globals.h"
#include "hook.h"

// Original: 0x007069C0, __fastcall-style callee returning uint32_t*, pops 4 bytes (ret 4).
CSF_FUNC(uint32_t*, __fastcall, C_0082d410_LookupFromTable, 0x007069C0, (void* self, void* edx, char a2));

// Original 0x007069F0: `sub ecx, 4; jmp 0x7069C0`. Steps back 4 bytes into the
// C_0082d410 object and tail-forwards to the lookup with the same argument.
static uint32_t* __fastcall C_0082d410_LookupFromEmbeddedBase(char* self, void* /*edx*/, char a2) {
    return C_0082d410_LookupFromTable(self - 4, nullptr, a2);
}

HOOK(0x007069F0, C_0082d410_LookupFromEmbeddedBase, "auto");
