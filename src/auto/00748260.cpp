#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_00858180_SetRespawnPointsMemoryCycle, 0x0074CBB0, (void* self, void* edx, char a2));

static char __fastcall C_00858180_RotateRespawnPointsAndUpdate(
    C_00858180* self, void* /*edx*/, char direction)
{
    const int count = CSF_AT(int8_t, self, 0x69);
    uint32_t*& points = direction == 1
        ? CSF_AT(uint32_t*, self, 0x350)
        : CSF_AT(uint32_t*, self, 0x358);

    if (points)
    {
        const uint32_t first = points[0];
        for (int i = 0; i < count - 1; ++i)
        {
            uint32_t* entries = points;
            entries[i] = entries[i + 1];
        }
        points[count - 1] = first;
    }

    const char result = static_cast<char>(Sub_64B250(self, nullptr));
    if (result)
        return C_00858180_SetRespawnPointsMemoryCycle(self, nullptr, direction);
    return result;
}

HOOK(0x00748260, C_00858180_RotateRespawnPointsAndUpdate, "auto");
