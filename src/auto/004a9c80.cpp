#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Copies the struct's first dword (offset 0) and field_0004 into the two-dword
// output buffer a2, then returns a2. __thiscall with one stack argument, ret 4.
static uint32_t* __fastcall S_004abb90_CopyPairToOut(S_004abb90* self, void* /*edx*/, uint32_t* outPair) {
    outPair[0] = CSF_AT(uint32_t, self, 0x0);
    outPair[1] = self->field_0004;
    return outPair;
}

HOOK(0x4A9C80, S_004abb90_CopyPairToOut, "auto");
