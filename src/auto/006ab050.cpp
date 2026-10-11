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

CSF_FUNC(int, __fastcall, sub_6A8110, 0x006A8110, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_6A9C30, 0x006A9C30, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_6AA6F0, 0x006AA6F0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, sub_6AAF50, 0x006AAF50, (void* self, void* edx, float a2));

// Runs the per-slot handler for each of the 10 task slots (pointer array at field_0328),
// then, when every slot reports idle (+4 == 0), resets the object state and calls sub_6AA6F0.
static int __fastcall C_0085fe28_DispatchTaskSlots(C_0085fe28* self, void* /*edx*/, int a2)
{
    bool allSlotsIdle = true;

    for (int slotOffset = 0; slotOffset < 0x28; slotOffset += 4)
    {
        S_006aaae0* task = *reinterpret_cast<S_006aaae0**>(self->field_0328 + slotOffset);
        if (task)
        {
            switch (task->field_0004)
            {
            case 1u:
                sub_6A9C30(task, nullptr, a2);
                break;
            case 2u:
                sub_6A8110(task, nullptr, a2);
                break;
            case 3u:
            {
                float asFloat;
                std::memcpy(&asFloat, &a2, sizeof(asFloat));
                sub_6AAF50(task, nullptr, asFloat);
                break;
            }
            }

            // The original reloads the slot pointer after the handler runs.
            S_006aaae0* taskAfter = *reinterpret_cast<S_006aaae0**>(self->field_0328 + slotOffset);
            if (CSF_AT(uint32_t, taskAfter, 4))
                allSlotsIdle = false;
        }
    }

    if (allSlotsIdle)
    {
        self->field_0008 = 2;
        self->field_000c = 0;
        self->field_0334 = 0;
        return sub_6AA6F0(self, nullptr);
    }
    return 0;
}

HOOK(0x6AB050, C_0085fe28_DispatchTaskSlots, "auto");
