#include <cstdint>
#include "globals.h"
#include "hook.h"

// Flushes pending bits from the 16-bit accumulator at +0x16B0 into the output
// buffer at +0x08. Writes at most two bytes per call, advances the write
// position at +0x14, and decrements the pending bit count at +0x16B4.
static uint32_t __cdecl ZlibLog_DrainBitAccumulator(hook::Regs* regs, const uint32_t* args) {
    (void)args;
    uint8_t* obj = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(regs->eax));
    const int32_t pendingBitCount = CSF_AT(int32_t, obj, 0x16B4);

    if (pendingBitCount == 16) {
        uint8_t* outBuffer = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, obj, 0x08)));
        uint32_t writePos = CSF_AT(uint32_t, obj, 0x14);
        outBuffer[writePos] = CSF_AT(uint8_t, obj, 0x16B0);

        uint8_t highByte = CSF_AT(uint8_t, obj, 0x16B1);
        uint32_t nextPos = CSF_AT(uint32_t, obj, 0x14) + 1;
        CSF_AT(uint32_t, obj, 0x14) = nextPos;
        outBuffer[nextPos] = highByte;
        CSF_AT(uint32_t, obj, 0x14) = CSF_AT(uint32_t, obj, 0x14) + 1;

        CSF_AT(uint16_t, obj, 0x16B0) = 0;
        CSF_AT(int32_t, obj, 0x16B4) = 0;
    } else if (pendingBitCount >= 8) {
        uint8_t* outBuffer = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, obj, 0x08)));
        uint32_t writePos = CSF_AT(uint32_t, obj, 0x14);
        outBuffer[writePos] = CSF_AT(uint8_t, obj, 0x16B0);

        CSF_AT(uint16_t, obj, 0x16B0) = static_cast<uint16_t>(CSF_AT(uint8_t, obj, 0x16B1));
        CSF_AT(uint32_t, obj, 0x14) = writePos + 1;
        CSF_AT(int32_t, obj, 0x16B4) = CSF_AT(int32_t, obj, 0x16B4) - 8;
    }

    return regs->eax;
}

HOOK_REGS(0x006A4420, ZlibLog_DrainBitAccumulator, 0, "core_zlib_log");
