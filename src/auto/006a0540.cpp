#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Appends a 16-bit value to the byte buffer big-endian (high byte, then low byte)
// and advances the write position by 2. Returns the writer object (eax on entry).
static uint32_t LogWriter_AppendBigEndianWord(hook::Regs* regs, const uint32_t* /*args*/) {
    uint8_t* writer = reinterpret_cast<uint8_t*>(regs->eax);
    uint8_t* buffer = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, writer, 0x08));
    uint16_t value = static_cast<uint16_t>(regs->ecx & 0xFFFF);

    uint32_t position = CSF_AT(uint32_t, writer, 0x14);
    buffer[position] = static_cast<uint8_t>(value >> 8);          // HIBYTE(a2)

    position = CSF_AT(uint32_t, writer, 0x14) + 1;
    CSF_AT(uint32_t, writer, 0x14) = position;
    buffer[position] = static_cast<uint8_t>(value & 0xFF);         // LOBYTE(a2)

    CSF_AT(uint32_t, writer, 0x14) = CSF_AT(uint32_t, writer, 0x14) + 1;
    return regs->eax;
}

HOOK_REGS(0x006A0540, LogWriter_AppendBigEndianWord, 0, "core_zlib_log");
