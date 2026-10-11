#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830,
         (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));

// Copies a spawn-point record field by field from source into self.
// The embedded string at +0x08 is assigned via AssignStringRange (full range, from position 0).
static char* __fastcall SpawnPoints_CopyRecord(char* self, void* /*edx*/, char* source) {
    CSF_AT(std::uint32_t, self, 0x00) = CSF_AT(std::uint32_t, source, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) = CSF_AT(std::uint32_t, source, 0x04);

    AssignStringRange_488830(self + 8, nullptr, source + 8, 0, 0xFFFFFFFF);

    CSF_AT(std::uint32_t, self, 0x24) = CSF_AT(std::uint32_t, source, 0x24);
    CSF_AT(std::uint8_t, self, 0x28) = CSF_AT(std::uint8_t, source, 0x28);
    CSF_AT(std::uint32_t, self, 0x2C) = CSF_AT(std::uint32_t, source, 0x2C);
    CSF_AT(std::uint32_t, self, 0x30) = CSF_AT(std::uint32_t, source, 0x30);
    CSF_AT(std::uint32_t, self, 0x34) = CSF_AT(std::uint32_t, source, 0x34);
    CSF_AT(std::uint32_t, self, 0x38) = CSF_AT(std::uint32_t, source, 0x38);
    CSF_AT(std::uint8_t, self, 0x3C) = CSF_AT(std::uint8_t, source, 0x3C);
    CSF_AT(std::uint8_t, self, 0x3D) = CSF_AT(std::uint8_t, source, 0x3D);
    CSF_AT(std::uint8_t, self, 0x3E) = CSF_AT(std::uint8_t, source, 0x3E);

    return self;
}

HOOK(0x005557C0, SpawnPoints_CopyRecord, "auto");
