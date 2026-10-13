#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Flushes the pending log characters (count at +0x16B4, bytes at +0x16B0/+0x16B1)
// into the log output buffer (pointer at +0x08, write index at +0x14), then clears
// the pending bytes and the count. Returns the object pointer unchanged in eax.
static uint32_t __cdecl LogWindow_FlushPendingChars(hook::Regs* regs, const uint32_t* args) {
    (void)args;

    uint8_t* self = reinterpret_cast<uint8_t*>(regs->eax);
    int32_t pendingCount = CSF_AT(int32_t, self, 0x16B4);

    if (pendingCount <= 8) {
        if (pendingCount > 0) {
            uint8_t* outputBuffer = CSF_AT(uint8_t*, self, 0x08);
            int32_t writeIndex = CSF_AT(int32_t, self, 0x14);
            outputBuffer[writeIndex] = CSF_AT(uint8_t, self, 0x16B0);
            CSF_AT(int32_t, self, 0x14) += 1;
        }
        CSF_AT(uint16_t, self, 0x16B0) = 0;
        CSF_AT(int32_t, self, 0x16B4) = 0;
    } else {
        uint8_t firstByte = CSF_AT(uint8_t, self, 0x16B0);
        uint8_t secondByte = CSF_AT(uint8_t, self, 0x16B1);

        uint8_t* outputBuffer = CSF_AT(uint8_t*, self, 0x08);
        int32_t writeIndex = CSF_AT(int32_t, self, 0x14);
        outputBuffer[writeIndex] = firstByte;

        writeIndex += 1;
        CSF_AT(int32_t, self, 0x14) = writeIndex;
        outputBuffer = CSF_AT(uint8_t*, self, 0x08);
        outputBuffer[writeIndex] = secondByte;

        CSF_AT(int32_t, self, 0x14) += 1;
        CSF_AT(uint16_t, self, 0x16B0) = 0;
        CSF_AT(int32_t, self, 0x16B4) = 0;
    }

    return regs->eax;
}

HOOK_REGS(0x6A44A0, LogWindow_FlushPendingChars, 0, "core_zlib_log");
