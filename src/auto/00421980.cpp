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

// Callees (bound by address, signatures per packet)
CSF_FUNC(int, __fastcall, TaskSlotIsLocked, 0x0041E460, (void* self, void* edx));
CSF_FUNC(double, __fastcall, TaskSlotAdvance, 0x0041E490, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, DispatchSceneObjectWithPayload, 0x0048E4D0, (void* self, void* edx, int payload));

// Scale constant used before the dispatch payload is formed
CSF_GLOBAL(float, g_PayloadScale, 0x0082E894);

// Advances the task slot at +0x244 by a2 and, unless the slot is locked,
// dispatches the advanced value to the owner object at +0x14.
static int __fastcall AiTaskStates_UpdateAndDispatch(void* self, void* /*edx*/, float a2)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);
    void* taskSlot = base + 0x244;

    int result = TaskSlotIsLocked(taskSlot, nullptr);
    if (!static_cast<uint8_t>(result))
    {
        double advanced = TaskSlotAdvance(taskSlot, nullptr, a2);
        float scaledPayload = static_cast<float>(advanced * g_PayloadScale);

        // The original stores the float into the pushed argument slot,
        // so the dispatch receives its bit pattern as the int payload.
        int payloadBits = 0;
        std::memcpy(&payloadBits, &scaledPayload, sizeof(payloadBits));

        void* owner = CSF_AT(void*, self, 0x14);
        return DispatchSceneObjectWithPayload(owner, nullptr, payloadBits);
    }
    return result;
}

HOOK(0x00421980, AiTaskStates_UpdateAndDispatch, "auto");
