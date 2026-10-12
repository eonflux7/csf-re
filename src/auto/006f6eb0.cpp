#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_006b0320_AssignPairTable, 0x006B0320, (S_006b0320* self, void* , int a2));

static char __fastcall S_006b0320_InitializeSceneInterval(
    S_006b0320* self, void* /*edx*/, float** pairs,
    int intervalStart, int intervalEnd, char flag68, char flag69)
{
    // The packet labels the callee address as a vtable reference.
    (void)CSF_VTBL(0x006B0320);

    self->field_0008 = 0;
    S_006b0320_AssignPairTable(self, nullptr, reinterpret_cast<int>(pairs));

    self->field_0010 = intervalStart;
    self->field_0069 = flag69;
    const uint32_t pairCount = self->field_0008;
    self->field_0014 = intervalEnd;
    self->field_0068 = flag68;

    if (pairCount <= 1)
    {
        self->field_0008 = 0;
        return 0;
    }

    CSF_AT(int, self, 0xA0) = intervalStart;
    CSF_AT(int, self, 0xAC) = intervalEnd;

    const float firstX = (*pairs)[0];
    CSF_AT(float, self, 0xA8) = firstX;
    CSF_AT(float, self, 0x9C) = firstX;

    const float firstY = (*pairs)[1];
    CSF_AT(float, self, 0xB0) = firstY;
    CSF_AT(float, self, 0xA4) = firstY;

    uint32_t index = 1;
    do
    {
        const float x =
            reinterpret_cast<float*>(self->field_0000)[2 * index];
        if (x < CSF_AT(float, self, 0x9C))
            CSF_AT(float, self, 0x9C) = x;
        else if (x > CSF_AT(float, self, 0xA8))
            CSF_AT(float, self, 0xA8) = x;

        const float y =
            reinterpret_cast<float*>(self->field_0000)[2 * index + 1];
        if (y < CSF_AT(float, self, 0xA4))
            CSF_AT(float, self, 0xA4) = y;
        else if (y > CSF_AT(float, self, 0xB0))
            CSF_AT(float, self, 0xB0) = y;

        ++index;
    } while (index < self->field_0008);

    return 1;
}

HOOK(0x006F6EB0, S_006b0320_InitializeSceneInterval, "auto");
