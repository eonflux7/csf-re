#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, CGuiTacticalMap_GetPositionBounds, 0x004368C0,
         (void* self, void* edx, float* a2, float* a3));

static char __fastcall CGuiTacticalMap_ClampMapPosition(
    void* self, void* /*edx*/, float* position)
{
    alignas(8) uint8_t minimumBytes[0xC] = {};
    alignas(8) uint8_t maximumBytes[0xC] = {};
    float* minimum = reinterpret_cast<float*>(minimumBytes);
    float* maximum = reinterpret_cast<float*>(maximumBytes);

    CGuiTacticalMap_GetPositionBounds(self, nullptr, maximum, minimum);

    char changed = 0;
    if (minimum[0] > position[0])
    {
        position[0] = minimum[0];
        changed = 1;
    }
    if (maximum[0] < position[0])
    {
        position[0] = maximum[0];
        changed = 1;
    }
    if (minimum[2] > position[2])
    {
        position[2] = minimum[2];
        changed = 1;
    }
    if (maximum[2] < position[2])
    {
        position[2] = maximum[2];
        return 1;
    }
    return changed;
}

HOOK(0x00436C90, CGuiTacticalMap_ClampMapPosition, "auto");
