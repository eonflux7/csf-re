#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Copies a 0x38-byte header block from the source record into the object at +4,
// then copies five selected fields from the source to fixed offsets of the object.
static char __fastcall C_00861120_Initialize(void* self, void* /*edx*/, void* source) {
    std::memcpy(static_cast<uint8_t*>(self) + 4, source, 0x38);
    CSF_AT(uint32_t, self, 0xE8) = CSF_AT(uint32_t, source, 0x10);
    CSF_AT(uint32_t, self, 0xEC) = CSF_AT(uint32_t, source, 0x14);
    CSF_AT(uint8_t, self, 0xDA) = CSF_AT(uint8_t, source, 0x18);
    CSF_AT(uint32_t, self, 0xF0) = CSF_AT(uint32_t, source, 0x04);
    CSF_AT(uint32_t, self, 0xF4) = CSF_AT(uint32_t, source, 0x08);
    return 1;
}

HOOK(0x006D6300, C_00861120_Initialize, "auto");
