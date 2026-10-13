#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

// Converts an unsigned 32-bit value to float the way the original does:
// a signed int->float conversion, plus the bias when the sign bit was set.
static float UnsignedToFloatWithBias(uint32_t value)
{
    const int32_t signedValue = static_cast<int32_t>(value);
    float result = static_cast<float>(signedValue);
    if (signedValue < 0)
        result += g_UnsignedIntegerBias;
    return result;
}

// Records one sample into the entry at index sampleIndex (16-byte entries):
// bumps the sample counter, optionally bumps the secondary counter, keeps the
// running maximum of the source value, and updates the running average.
static int __fastcall RecordParticleSampleAndUpdateAverage(S_007030d0* self, void* /*edx*/, int sampleIndex, char countFlag)
{
    const int v3 = 16 * sampleIndex;
    const uint32_t entry = self->field_0010 + static_cast<uint32_t>(v3);
    uint8_t* entryPtr = reinterpret_cast<uint8_t*>(entry);

    const uint32_t newCount = CSF_AT(uint32_t, entryPtr, 0) + 1;
    CSF_AT(uint32_t, entryPtr, 0) = newCount;
    const uint32_t sampleCount = newCount;

    if (countFlag)
        CSF_AT(uint32_t, entryPtr, 4) += 1;

    uint8_t* sourceBase = reinterpret_cast<uint8_t*>(self->field_0000);
    uint32_t sample = CSF_AT(uint32_t, sourceBase, v3 + 8);
    if (sample > CSF_AT(uint32_t, entryPtr, 8))
        CSF_AT(uint32_t, entryPtr, 8) = sample;

    const float previousAverage = CSF_AT(float, entryPtr, 0xC);
    const uint32_t sampleAgain = CSF_AT(uint32_t, sourceBase, v3 + 8);
    const float updatedAverage =
        (UnsignedToFloatWithBias(sampleCount - 1) * previousAverage
         + UnsignedToFloatWithBias(sampleAgain))
        / UnsignedToFloatWithBias(sampleCount);
    CSF_AT(float, entryPtr, 0xC) = updatedAverage;

    return static_cast<int>(entry);
}

HOOK(0x00703280, RecordParticleSampleAndUpdateAverage, "auto");
