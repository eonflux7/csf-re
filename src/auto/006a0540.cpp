#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Appends a 16-bit value to a byte buffer in big-endian order (high byte first).
// The original is usercall: object in eax, value in cx, returns the object in eax, pops 0 bytes.
// Layout as accessed by the original: buffer pointer at +0x08, write position at +0x14.
static void* __cdecl LogWriter_AppendBigEndianWord(void* self, uint16_t value)
{
    uint8_t highByte = static_cast<uint8_t>(value >> 8);
    uint8_t lowByte = static_cast<uint8_t>(value & 0xFF);

    CSF_AT(uint8_t*, self, 0x08)[CSF_AT(int, self, 0x14)] = highByte;

    CSF_AT(int, self, 0x14) = CSF_AT(int, self, 0x14) + 1;
    CSF_AT(uint8_t*, self, 0x08)[CSF_AT(int, self, 0x14)] = lowByte;

    CSF_AT(int, self, 0x14) = CSF_AT(int, self, 0x14) + 1;
    return self;
}

HOOK(0x006A0540, LogWriter_AppendBigEndianWord, "auto");
