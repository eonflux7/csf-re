#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, ov_read, 0x0081DD16,
         (void* decoder, char* output, int length, int bigEndian,
          int sampleBytes, int signedSamples, int* bitstream));

CSF_GLOBAL(float, g_MaximumHeadTimeStep, 0x0082DC68);

static char __fastcall S_0070e170_FillPcmBuffer(
    S_0070e170* self, void* /*edx*/)
{
    int remaining = static_cast<int>(self->field_001c);
    std::memset(reinterpret_cast<void*>(self->field_0014), 0,
                static_cast<std::size_t>(remaining));

    alignas(8) uint8_t bitstreamBytes[0x8];
    int& bitstream = *reinterpret_cast<int*>(bitstreamBytes);
    char complete = 1;

    for (;;)
    {
        const uint32_t outputAddress =
            self->field_0014 + self->field_001c -
            static_cast<uint32_t>(remaining);
        void* decoder = reinterpret_cast<void*>(self->field_003c);
        bitstream = 0;

        const int bytesRead = ov_read(
            decoder, reinterpret_cast<char*>(outputAddress),
            remaining, 0, 2, 1, &bitstream);

        if (bytesRead == 0)
        {
            complete = 0;
            break;
        }

        if (bytesRead > 0)
        {
            remaining -= bytesRead;
            self->field_0038 = 1;
            if (remaining <= 0)
                break;
        }
    }

    if (self->field_0008 < g_MaximumHeadTimeStep)
    {
        std::memset(reinterpret_cast<void*>(self->field_0014), 0,
                    self->field_001c);
    }

    return complete;
}

HOOK(0x0070E4A0, S_0070e170_FillPcmBuffer, "auto");
