#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, MissionScript_AllocRecord8, 0x00471450, (uint32_t a1));

static int __fastcall MissionScript_EnsureSlotRecord(void* self, void* /*edx*/, int index)
{
    uint32_t* slots = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x4));
    if (slots[index] == 0)
    {
        int record = MissionScript_AllocRecord8(8);
        if (record == 0)
        {
            slots = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x4));
            slots[index] = 0;
        }
        else
        {
            uint8_t* rec = reinterpret_cast<uint8_t*>(record);
            CSF_AT(uint16_t, rec, 0) = 0;
            CSF_AT(uint16_t, rec, 2) = 0xFFFF;
            CSF_AT(uint32_t, rec, 4) = 0;
            slots = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x4));
            slots[index] = static_cast<uint32_t>(record);
        }
    }
    slots = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x4));
    return static_cast<int>(slots[index]);
}

HOOK(0x0046F4D0, MissionScript_EnsureSlotRecord, "auto");
