#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_AxisY, 0x008A7D78);
CSF_GLOBAL(float, g_AxisY_Y, 0x008A7D7C);
CSF_GLOBAL(float, g_AxisY_Z, 0x008A7D80);

// Advances the per-entry float at +0x14 of the 28-byte entry array (pointer at self+0x0C),
// rewrites the entry's two words at +0x10/+0x18 unchanged, then notifies the entry's
// attached object (vtable slot 17) with the axis vector scaled by delta.
static uint32_t* __fastcall AdvanceAxisEntry(void* self, void* /*edx*/, int entryIndex, float delta)
{
    uint8_t* entryArray = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x0C));
    uint8_t* entry = entryArray + 28 * entryIndex;

    CSF_AT(float, entry, 0x14) = delta + CSF_AT(float, entry, 0x14);

    uint8_t* result = entry + 0x10;
    uint32_t wordAt10 = CSF_AT(uint32_t, result, 0x00);
    CSF_AT(uint32_t, result, 0x00) = wordAt10;
    uint32_t wordAt18 = CSF_AT(uint32_t, result, 0x08);
    CSF_AT(uint32_t, result, 0x08) = wordAt18;

    void* attachedObject = reinterpret_cast<void*>(CSF_AT(uint32_t, entry, 0x00));
    if (attachedObject == nullptr)
    {
        return reinterpret_cast<uint32_t*>(result);
    }

    float axisVector[3];
    axisVector[0] = g_AxisY * delta;
    axisVector[1] = g_AxisY_Y * delta;
    axisVector[2] = g_AxisY_Z * delta;

    return ((uint32_t*(__fastcall*)(void*, void*, float*))CSF_AT(void**, attachedObject, 0)[17])(
        attachedObject, nullptr, axisVector);
}

HOOK(0x4080C0, AdvanceAxisEntry, "auto");
