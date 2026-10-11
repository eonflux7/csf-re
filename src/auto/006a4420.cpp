#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Drains the zlib log bit accumulator into the output byte buffer.
// Layout (offsets relative to the log object):
//   +0x08    uint8_t* output buffer
//   +0x14    int      write position in the output buffer
//   +0x16B0  uint8_t  pending byte 0 (low byte of the pending 16-bit word)
//   +0x16B1  uint8_t  pending byte 1 (high byte of the pending 16-bit word)
//   +0x16B4  int      number of valid bits held in the pending word
// Returns the object pointer unchanged (the original leaves eax = result).
static int __fastcall ZlibLog_DrainBitAccumulator(void* self, void* /*edx*/)
{
    int pendingBitCount = CSF_AT(int, self, 0x16B4);

    if (pendingBitCount == 16)
    {
        // Two full bytes pending: emit both, then clear the accumulator.
        uint8_t* outputBuffer = CSF_AT(uint8_t*, self, 0x08);
        CSF_AT(uint8_t, outputBuffer + CSF_AT(int, self, 0x14), 0) =
            CSF_AT(uint8_t, self, 0x16B0);

        uint8_t highByte = CSF_AT(uint8_t, self, 0x16B1);
        int writePos = CSF_AT(int, self, 0x14) + 1;
        CSF_AT(int, self, 0x14) = writePos;
        outputBuffer[writePos] = highByte;
        CSF_AT(int, self, 0x14) += 1;

        CSF_AT(uint16_t, self, 0x16B0) = 0;
        CSF_AT(int, self, 0x16B4) = 0;
    }
    else if (pendingBitCount >= 8)
    {
        // One full byte pending: emit the low byte, shift the high byte down.
        uint8_t* outputBuffer = CSF_AT(uint8_t*, self, 0x08);
        outputBuffer[CSF_AT(int, self, 0x14)] = CSF_AT(uint8_t, self, 0x16B0);

        int oldWritePos = CSF_AT(int, self, 0x14);
        CSF_AT(uint16_t, self, 0x16B0) = CSF_AT(uint8_t, self, 0x16B1);
        CSF_AT(int, self, 0x14) = oldWritePos + 1;
        CSF_AT(int, self, 0x16B4) = CSF_AT(int, self, 0x16B4) - 8;
    }

    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x6A4420, ZlibLog_DrainBitAccumulator, "auto");
