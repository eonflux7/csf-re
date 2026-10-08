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

// Original callees (bound by address).
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, LookupObjectByGlobalId, 0x00652160, (int globalId, void* /*edx*/));

// Returns the settings object stored in CMenuVideoOptions::field_0024 (re-read on every access, as the original does).
static void* CMenuVideoOptions_GetSettingsObject(CMenuVideoOptions* self)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0024));
}

// Calls a no-argument __thiscall getter through the object's vtable slot (slot = byte offset / 4).
template <typename Ret>
static Ret CallVtableGetter(void* obj, int slot)
{
    using Getter = Ret(__fastcall*)(void*, void*);
    return reinterpret_cast<Getter>(CSF_AT(void**, obj, 0)[slot])(obj, nullptr);
}

// Fills a 48-byte snapshot of the current video settings.
// If the settings object is present, every field is pulled from its getters.
// Otherwise the snapshot holds two values from the object found by the global id.
// Returns the last value written (0 when no fallback object exists).
static int __fastcall CMenuVideoOptions_FillSettingsSnapshot(CMenuVideoOptions* self, void* /*edx*/, uint8_t* outSnapshot)
{
    if (self->field_0024 != 0)
    {
        // Record pointer: first two dwords are copied from the object returned by slot 0x20.
        uint32_t* record = static_cast<uint32_t*>(
            CallVtableGetter<void*>(CMenuVideoOptions_GetSettingsObject(self), 8));
        CSF_AT(uint32_t, outSnapshot, 0x00) = record[0];
        CSF_AT(uint32_t, outSnapshot, 0x04) = record[1];

        CSF_AT(uint32_t, outSnapshot, 0x08) =
            CallVtableGetter<uint32_t>(CMenuVideoOptions_GetSettingsObject(self), 6);
        CSF_AT(float, outSnapshot, 0x0C) =
            CallVtableGetter<float>(CMenuVideoOptions_GetSettingsObject(self), 10);
        CSF_AT(uint8_t, outSnapshot, 0x10) =
            CallVtableGetter<uint8_t>(CMenuVideoOptions_GetSettingsObject(self), 12);
        CSF_AT(uint8_t, outSnapshot, 0x11) =
            CallVtableGetter<uint8_t>(CMenuVideoOptions_GetSettingsObject(self), 14);
        CSF_AT(uint32_t, outSnapshot, 0x14) =
            CallVtableGetter<uint32_t>(CMenuVideoOptions_GetSettingsObject(self), 16);
        CSF_AT(uint32_t, outSnapshot, 0x18) =
            CallVtableGetter<uint32_t>(CMenuVideoOptions_GetSettingsObject(self), 18);
        CSF_AT(uint32_t, outSnapshot, 0x1C) =
            CallVtableGetter<uint32_t>(CMenuVideoOptions_GetSettingsObject(self), 20);
        CSF_AT(uint32_t, outSnapshot, 0x20) =
            CallVtableGetter<uint32_t>(CMenuVideoOptions_GetSettingsObject(self), 22);
        CSF_AT(uint32_t, outSnapshot, 0x24) =
            CallVtableGetter<uint32_t>(CMenuVideoOptions_GetSettingsObject(self), 24);
        CSF_AT(uint32_t, outSnapshot, 0x28) =
            CallVtableGetter<uint32_t>(CMenuVideoOptions_GetSettingsObject(self), 26);

        uint32_t lastValue =
            CallVtableGetter<uint32_t>(CMenuVideoOptions_GetSettingsObject(self), 28);
        CSF_AT(uint32_t, outSnapshot, 0x2C) = lastValue;
        return static_cast<int>(lastValue);
    }

    // Fallback: object looked up through the global id.
    int globalId = GetGlobalInt();
    void* foundObject = LookupObjectByGlobalId(globalId, nullptr);
    if (foundObject == nullptr)
    {
        return 0;
    }

    uint32_t secondValue = CallVtableGetter<uint32_t>(foundObject, 9);
    uint32_t firstValue = CallVtableGetter<uint32_t>(foundObject, 8);
    CSF_AT(uint32_t, outSnapshot, 0x00) = firstValue;
    CSF_AT(uint32_t, outSnapshot, 0x04) = secondValue;
    return static_cast<int>(firstValue);
}

HOOK(0x00549760, CMenuVideoOptions_FillSettingsSnapshot, "auto");
