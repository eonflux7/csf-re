#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Log stream field offsets, as accessed by the original:
//   +0x08 : buffer base address
//   +0x14 : write index into the buffer
//   +0x16AC : stream state, reset to 8

// LogWindow_FlushPendingChars takes its argument in eax and pushes nothing.
static int* LogWindow_FlushPendingChars(int streamArg) {
    hook::Regs r{};
    r.eax = static_cast<uint32_t>(streamArg);
    uint32_t result = hook::call_regs(CSF_ADDR_FN(0x006A44A0), &r, nullptr, 0);
    return reinterpret_cast<int*>(result);
}

// Appends one byte to the stream buffer and advances the write index.
static void LogStream_AppendByte(int* stream, uint8_t value) {
    uint32_t base = CSF_AT(uint32_t, stream, 0x08);
    int index = CSF_AT(int, stream, 0x14);
    *reinterpret_cast<uint8_t*>(base + index) = value;
    CSF_AT(int, stream, 0x14) = index + 1;
}

// Resets the log stream state to 8 and appends an optional zlib stored-block
// LEN/NLEN header, followed by a raw copy of `length` bytes from `source`.
// Argument mapping: eax = stream, edx = source, ecx = length, [esp+4] = withHeader flag.
static int* LogStream_WriteStoredBlock(int stream, uint8_t* source, int length, int withHeader) {
    int* result = LogWindow_FlushPendingChars(stream);
    CSF_AT(int, result, 0x16AC) = 8;

    if (withHeader) {
        uint16_t len = static_cast<uint16_t>(length);
        uint16_t nlen = static_cast<uint16_t>(~len);
        LogStream_AppendByte(result, static_cast<uint8_t>(len));
        LogStream_AppendByte(result, static_cast<uint8_t>(len >> 8));
        LogStream_AppendByte(result, static_cast<uint8_t>(nlen));
        LogStream_AppendByte(result, static_cast<uint8_t>(nlen >> 8));
    }

    if (length) {
        int remaining = length;
        do {
            int current = remaining;
            LogStream_AppendByte(result, *source++);
            --remaining;
        } while (remaining != 0);
    }
    return result;
}

HOOK(0x006A4510, LogStream_WriteStoredBlock, "auto");
