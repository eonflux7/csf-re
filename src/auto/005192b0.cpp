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

// Advances this->field_0010 by a step derived from the int at a2+8 (a
// signed-divide-by-constant of that value), clamping it to zero when negative.
// Returns the step, or 0 when this->field_000c is zero.
static unsigned int __fastcall C_00840ca8_UpdateAccumulatorFromTimeDelta(C_00840ca8* self, void* /*edx*/, int a2)
{
    unsigned int result = self->field_000c;
    if (result)
    {
        void* source = reinterpret_cast<void*>(static_cast<uintptr_t>(a2));
        int32_t sourceValue = CSF_AT(int, source, 0x8);

        // Signed high 32 bits of sourceValue * 0x77777777, then subtract the source value.
        int64_t product = static_cast<int64_t>(sourceValue) * static_cast<int64_t>(2004318071LL);
        int32_t highPart = static_cast<int32_t>(product >> 32);
        int32_t difference = highPart - sourceValue;

        // Arithmetic shift by 6 plus one when the difference is negative (round toward zero).
        uint32_t step = static_cast<uint32_t>((difference >> 6) +
                                              static_cast<int32_t>(static_cast<uint32_t>(difference) >> 31));
        result = step;

        uint32_t accumulated = self->field_0010;
        self->field_0010 = result + accumulated;
        if (static_cast<int32_t>(result + accumulated) < 0)
        {
            self->field_0010 = 0;
        }
    }
    return result;
}

HOOK(0x5192B0, C_00840ca8_UpdateAccumulatorFromTimeDelta, "auto");
