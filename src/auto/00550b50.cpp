#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static uint8_t* __fastcall MpGameModes_CopySpawnPointSettings(
    uint8_t* self, void* /*edx*/, const uint8_t* source)
{
    const uint8_t* read = source + 0x87;
    uint8_t* write = self + 0x87;
    uint8_t character;
    do {
        character = *read;
        *write = character;
        ++read;
        ++write;
    } while (character);

    read = source + 7;
    write = self + 7;
    do {
        character = *read;
        *write = character;
        ++read;
        ++write;
    } while (character);

    CSF_AT(uint8_t, self, 6) = CSF_AT(uint8_t, source, 6);
    CSF_AT(uint8_t, self, 5) = CSF_AT(uint8_t, source, 5);

    read = source + 0x107;
    write = self + 0x107;
    do {
        character = *read;
        *write = character;
        ++read;
        ++write;
    } while (character);

    CSF_AT(uint8_t, self, 4) = CSF_AT(uint8_t, source, 4);
    CSF_AT(uint8_t, self, 0) = CSF_AT(uint8_t, source, 0);
    CSF_AT(uint8_t, self, 1) = CSF_AT(uint8_t, source, 1);
    CSF_AT(uint8_t, self, 2) = CSF_AT(uint8_t, source, 2);
    CSF_AT(uint8_t, self, 3) = CSF_AT(uint8_t, source, 3);

    CSF_AT(uint32_t, self, 0x188) = CSF_AT(uint32_t, source, 0x188);
    CSF_AT(uint16_t, self, 0x196) = CSF_AT(uint16_t, source, 0x196);
    CSF_AT(uint32_t, self, 0x190) = CSF_AT(uint32_t, source, 0x190);
    CSF_AT(uint16_t, self, 0x198) = CSF_AT(uint16_t, source, 0x198);
    CSF_AT(uint16_t, self, 0x194) = CSF_AT(uint16_t, source, 0x194);
    CSF_AT(uint16_t, self, 0x19A) = CSF_AT(uint16_t, source, 0x19A);
    CSF_AT(uint32_t, self, 0x18C) = CSF_AT(uint32_t, source, 0x18C);
    CSF_AT(uint16_t, self, 0x19C) = CSF_AT(uint16_t, source, 0x19C);

    return self;
}

HOOK(0x00550B50, MpGameModes_CopySpawnPointSettings, "auto");
