#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Compares a candidate video-mode record (a2, five dwords) against the
// reference record at a3. When a3's flag byte at +0x14 is set, all of
// a2[0], a2[1], a2[2] must match a3 and a2[3] must equal 1. When it is clear,
// the record is accepted if a2[3] is zero. On success the mode value a2[4] is
// stored in this->field_0030 and 1 is returned, otherwise 0.
static char __fastcall C_00861460_TryAcceptVideoModeEntry(C_00861460* self, void* /*edx*/, uint32_t* a2, int a3)
{
    char result = 0;
    if (*reinterpret_cast<const uint8_t*>(a3 + 20))
    {
        const uint32_t* reference = reinterpret_cast<const uint32_t*>(a3);
        if (a2[0] == reference[0] &&
            a2[1] == reference[1] &&
            a2[2] == *reinterpret_cast<const uint32_t*>(a3 + 16) &&
            a2[3] == 1)
        {
            self->field_0030 = a2[4];
            return 1;
        }
    }
    else if (!a2[3])
    {
        self->field_0030 = a2[4];
        return 1;
    }
    return result;
}

HOOK(0x006DC1F0, C_00861460_TryAcceptVideoModeEntry, "auto");
