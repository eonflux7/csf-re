#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Flushes the pending bytes (up to two) held in the log line state object.
// Stores the buffered bytes into the output buffer, then resets the pending count and pending bytes.
static int LogWindow_FlushPendingChars(int result) {
    uint8_t* self = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(result));
    int pendingCount = CSF_AT(int, self, 0x16B4);

    if (pendingCount <= 8) {
        if (pendingCount > 0) {
            uint8_t* outBuffer = CSF_AT(uint8_t*, self, 8);
            int& writeIndex = CSF_AT(int, self, 0x14);
            outBuffer[writeIndex] = CSF_AT(uint8_t, self, 0x16B0);
            ++writeIndex;
        }
        CSF_AT(uint16_t, self, 0x16B0) = 0;
        CSF_AT(int, self, 0x16B4) = 0;
    } else {
        uint8_t* outBuffer = CSF_AT(uint8_t*, self, 8);
        int& writeIndex = CSF_AT(int, self, 0x14);
        outBuffer[writeIndex] = CSF_AT(uint8_t, self, 0x16B0);

        uint8_t secondByte = CSF_AT(uint8_t, self, 0x16B1);
        writeIndex = writeIndex + 1;
        outBuffer[writeIndex] = secondByte;
        ++writeIndex;

        CSF_AT(uint16_t, self, 0x16B0) = 0;
        CSF_AT(int, self, 0x16B4) = 0;
    }

    return result;
}

HOOK(0x6A44A0, LogWindow_FlushPendingChars, "auto");
