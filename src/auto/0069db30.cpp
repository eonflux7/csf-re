#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t*, __cdecl, InitializeToInvalidAndReturn, 0x0069D7A0, (uint32_t* a1));
CSF_FUNC(int, __cdecl, NetProfile_AccumulateChecksumByte, 0x0069D7B0, (uint8_t a1, int* a2));

static int __stdcall MpRules_ComputeFileChecksum(int a1, unsigned int a2)
{
    int checksum = 0;
    InitializeToInvalidAndReturn(reinterpret_cast<uint32_t*>(&checksum));

    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(a1);
    for (unsigned int i = 0; i < a2; ++i)
    {
        NetProfile_AccumulateChecksumByte(bytes[i], &checksum);
    }
    return checksum;
}

HOOK(0x0069DB30, MpRules_ComputeFileChecksum, "auto");
