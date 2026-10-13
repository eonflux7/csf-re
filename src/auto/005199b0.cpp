#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Fills a 18-byte record: three words, a dword copied from *payloadSource,
// a flag byte, and a cleared byte. Returns the record.
static uint32_t* __fastcall InitializeRecordWithPayload(uint32_t* self, void* /*edx*/,
                                                        uint32_t firstValue,
                                                        uint32_t secondValue,
                                                        uint32_t thirdValue,
                                                        const uint32_t* payloadSource,
                                                        char flagByte) {
    CSF_AT(uint32_t, self, 0x00) = firstValue;
    CSF_AT(uint32_t, self, 0x04) = secondValue;
    CSF_AT(uint32_t, self, 0x08) = thirdValue;
    CSF_AT(uint32_t, self, 0x0C) = *payloadSource;
    CSF_AT(char, self, 0x10) = flagByte;
    CSF_AT(uint8_t, self, 0x11) = 0;
    return self;
}

HOOK(0x005199B0, InitializeRecordWithPayload, "auto");
