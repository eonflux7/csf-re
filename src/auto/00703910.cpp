#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, RpPrtStdPropTabGetPropOffset, 0x0057C8B0, (int a1, int a2));

CSF_GLOBAL(uint32_t, g_BlackClearColor, 0x008BB430);
CSF_GLOBAL(uint32_t, g_DefaultParamA_8BB42C, 0x008BB42C);

// Resets the PrtStd property record (property id 9) of a particle to its default values.
// Returns the particle pointer that was passed in (a2).
static int PrtStd_InitParticleRecordDefaults(int a1, int a2)
{
    [[maybe_unused]] int unusedA1 = a1;

    uint8_t* particle = reinterpret_cast<uint8_t*>(a2);
    uint8_t* particleInner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, particle, 0x0C));
    int propertyTable = static_cast<int>(CSF_AT(uint32_t, particleInner, 0x10));

    int recordOffset = RpPrtStdPropTabGetPropOffset(propertyTable, 9);
    uint8_t* record = particle + recordOffset;

    CSF_AT(uint32_t, record, 0) = 0;
    CSF_AT(uint8_t, record, 5) = 0;
    CSF_AT(uint8_t, record, 4) = 0;
    CSF_AT(uint32_t, record, 6) = g_BlackClearColor;
    CSF_AT(uint32_t, record, 10) = g_DefaultParamA_8BB42C;
    CSF_AT(uint8_t, record, 14) = 0xFF;
    CSF_AT(uint8_t, record, 15) = 0;
    CSF_AT(uint8_t, record, 16) = 0;
    CSF_AT(uint32_t, record, 20) = 0;
    CSF_AT(uint32_t, record, 24) = 0;
    CSF_AT(uint32_t, record, 28) = 0;
    CSF_AT(uint32_t, record, 32) = 0;
    CSF_AT(uint32_t, record, 36) = 0;
    CSF_AT(uint8_t, record, 40) = 0;
    CSF_AT(uint8_t, record, 41) = 1;
    CSF_AT(uint8_t, record, 42) = 1;
    CSF_AT(uint32_t, record, 44) = 0;

    return a2;
}

HOOK(0x00703910, PrtStd_InitParticleRecordDefaults, "auto");
