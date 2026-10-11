#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_AxisX, 0x008A7D6C);
CSF_GLOBAL(float, g_AxisX_Y, 0x008A7D70);
CSF_GLOBAL(float, g_AxisX_Z, 0x008A7D74);

// Advances the per-entry float at +0x10 of the 28-byte entry array (pointer at self+0x0C),
// rewrites the entry's two words at +0x14/+0x18 unchanged, then notifies the entry's
// attached object (vtable slot 17) with the X-axis vector scaled by delta.
// Returns the address of the entry's +0x10 slot, or the vtable call's result when notified.
static int __fastcall AdvanceAxisXEntry(void* self, void* /*edx*/, int entryIndex, float delta)
{
    uint8_t* entryArray = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x0C));
    uint8_t* entry = entryArray + 28 * entryIndex;

    CSF_AT(float, entry, 0x10) = delta + CSF_AT(float, entry, 0x10);

    uint8_t* result = entry + 0x10;
    uint32_t wordAt14 = CSF_AT(uint32_t, result, 0x04);
    CSF_AT(uint32_t, result, 0x04) = wordAt14;
    uint32_t wordAt18 = CSF_AT(uint32_t, result, 0x08);
    CSF_AT(uint32_t, result, 0x08) = wordAt18;

    void* attachedObject = reinterpret_cast<void*>(CSF_AT(uint32_t, entry, 0x00));
    if (attachedObject == nullptr)
    {
        return static_cast<int>(reinterpret_cast<uintptr_t>(result));
    }

    float axisVector[3];
    axisVector[0] = g_AxisX * delta;
    axisVector[1] = g_AxisX_Y * delta;
    axisVector[2] = g_AxisX_Z * delta;

    return ((int(__fastcall*)(void*, void*, float*))CSF_AT(void**, attachedObject, 0)[17])(
        attachedObject, nullptr, axisVector);
}

HOOK(0x408050, AdvanceAxisXEntry, "auto");
