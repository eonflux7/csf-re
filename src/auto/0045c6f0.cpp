#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Copies the active value (self+0x58) into the output slot when self+0x28 == 4;
// otherwise writes 0 to the output slot. Returns the value written.
static int __fastcall ScriptCommands_CopyActiveValueToOutput(void* self, void* /*edx*/, int a2, int a3, int a4) {
    (void)a3;
    (void)a4;
    int result = 0;
    if (CSF_AT(int, self, 0x28) == 4) {
        result = CSF_AT(int, self, 0x58);
        uint8_t* outSlot = reinterpret_cast<uint8_t*>(a2);
        uint8_t* outTarget = reinterpret_cast<uint8_t*>(CSF_AT(uintptr_t, outSlot, 4));
        CSF_AT(int, outTarget, 4) = result;
    } else {
        uint8_t* outSlot = reinterpret_cast<uint8_t*>(a2);
        uint8_t* outTarget = reinterpret_cast<uint8_t*>(CSF_AT(uintptr_t, outSlot, 4));
        CSF_AT(int, outTarget, 4) = 0;
    }
    return result;
}

HOOK(0x45C6F0, ScriptCommands_CopyActiveValueToOutput, "auto");
