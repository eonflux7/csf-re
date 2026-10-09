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

CSF_FUNC(unsigned int, __fastcall, SetSlotValue, 0x00472350, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, GetSlotCountLimit, 0x00672AC0, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);

// Applies a metric value read from the record pair to a per-type slot table.
// Original: __thiscall, 3 stack args, ret 12.
static void __fastcall ApplyMetricToSceneEntry(void* self, void* /*edx*/, int /*a2*/, int /*a3*/, uint32_t* metricRecord)
{
    (void)sizeof(CSF_VTBL(0x00472350));

    // Key index stored in the object referenced by metricRecord[0].
    void* keyObject = reinterpret_cast<void*>(metricRecord[0]);
    uint32_t keyIndex = CSF_AT(uint32_t, keyObject, 0x8);

    // Default table owner comes from self; flagged keys use the current scene's table.
    uint32_t tableOwner = CSF_AT(uint32_t, self, 0x20);
    if ((0x800000u & keyIndex) != 0)
    {
        void* sceneChild = reinterpret_cast<void*>(CSF_AT(uint32_t, g_pCurrentScene, 0xBC));
        tableOwner = CSF_AT(uint32_t, sceneChild, 0x38);
        keyIndex &= ~0x800000u;
    }

    // Each slot entry is 12 bytes; the pointer field at +4 is the slot object.
    uint32_t tableBase = CSF_AT(uint32_t, reinterpret_cast<void*>(tableOwner), 0x8);
    uint8_t* slotEntry = reinterpret_cast<uint8_t*>(tableBase + 12u * keyIndex);

    // Metric value: metricRecord[1] -> object at +4 -> float at +4.
    void* metricOwner = reinterpret_cast<void*>(metricRecord[1]);
    void* metricObject = reinterpret_cast<void*>(CSF_AT(uint32_t, metricOwner, 0x4));
    float metricValue = CSF_AT(float, metricObject, 0x4);

    if (metricValue != g_MissingMetricValue)
    {
        uint64_t wideValue = static_cast<uint64_t>(metricValue);
        uint32_t metricInt = static_cast<uint32_t>(wideValue);

        uint32_t slotLimit = static_cast<uint32_t>(GetSlotCountLimit(
            reinterpret_cast<void*>(CSF_AT(uint32_t, slotEntry, 0x4)),
            reinterpret_cast<void*>(static_cast<uint32_t>(wideValue >> 32))));

        if (metricInt < slotLimit)
        {
            SetSlotValue(reinterpret_cast<void*>(CSF_AT(uint32_t, slotEntry, 0x4)), nullptr, metricInt);
        }
    }
}

HOOK(0x004648B0, ApplyMetricToSceneEntry, "auto");
