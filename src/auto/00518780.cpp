#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, InitializeToInvalidAndReturn, 0x0069D7A0, (uint32_t* a1));
CSF_FUNC(int, __cdecl, NetProfile_AccumulateChecksumByte, 0x0069D7B0, (uint8_t a1, int* a2));

static int __fastcall S_00518780_CalculateChecksum(
    S_00518780* self, void* /*edx*/, int* checksum)
{
    InitializeToInvalidAndReturn(reinterpret_cast<uint32_t*>(checksum));

    for (int i = 0; i < 33; ++i)
        NetProfile_AccumulateChecksumByte(
            CSF_AT(uint8_t, self, 0x08 + i), checksum);

    for (int i = 0; i < 17; ++i)
        NetProfile_AccumulateChecksumByte(
            CSF_AT(uint8_t, self, 0x29 + i), checksum);

    NetProfile_AccumulateChecksumByte(self->field_003a, checksum);
    NetProfile_AccumulateChecksumByte(self->field_0000, checksum);
    NetProfile_AccumulateChecksumByte(self->field_0040, checksum);
    NetProfile_AccumulateChecksumByte(self->field_0044, checksum);

    for (int i = 0; i < 513; ++i)
        NetProfile_AccumulateChecksumByte(
            CSF_AT(uint8_t, self, 0x48 + i), checksum);

    NetProfile_AccumulateChecksumByte(self->field_0249, checksum);
    NetProfile_AccumulateChecksumByte(self->field_024a, checksum);
    return NetProfile_AccumulateChecksumByte(self->field_024b, checksum);
}

HOOK(0x00518780, S_00518780_CalculateChecksum, "auto");
