#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

CSF_FUNC(uint8_t*, __fastcall, sub_4F0F50, 0x004F0F50, (void* self, void* edx));

// Returns true when the light slot selected by a2 (0 = +0x2C, 1 = +0x1C, 2 = +0x0C)
// has its state word at +8 non-zero. Before that, if the enable flag at +0x40 is set and
// the timer at +0x3C has gone negative, it is reset to 0.5 and sub_4F0F50 runs.
static bool __fastcall SceneLights_IsSlotStateActive(S_004f0cd0* self, void* /*edx*/, int slotIndex)
{
    if (self->field_0040)
    {
        if (self->field_003c < g_fZero)
        {
            self->field_003c = 0.5f;
            sub_4F0F50(self, nullptr);
        }
    }

    uint8_t* slotBase = nullptr;
    if (slotIndex == 0)
    {
        slotBase = reinterpret_cast<uint8_t*>(self) + 0x2C;
    }
    else if (slotIndex == 1)
    {
        slotBase = reinterpret_cast<uint8_t*>(self) + 0x1C;
    }
    else if (slotIndex == 2)
    {
        slotBase = reinterpret_cast<uint8_t*>(self) + 0x0C;
    }
    else
    {
        return false;
    }

    if (slotBase)
    {
        uint32_t stateWord = CSF_AT(uint32_t, slotBase, 0x08);
        return stateWord != 0;
    }
    return false;
}

HOOK(0x4F1340, SceneLights_IsSlotStateActive, "auto");
