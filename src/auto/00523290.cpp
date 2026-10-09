#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, ApplyFloatToGlobalObject, 0x005221E0, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, GrowSoundEntryBuffer, 0x00522BA0, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(int32_t, g_Value_8B0674, 0x008B0674);

// Appends a (tag, a3, a2) entry to the buffer, or, when a2 matches an existing
// entry via ApplyFloatToGlobalObject, overwrites that entry's second dword with a3.
static int __fastcall SoundManager_AddOrUpdateEntry(S_00522ba0* self, void* /*edx*/, float a2, int a3)
{
    // Referenced vtable address from the original (no vtable store in this function).
    (void)CSF_VTBL(0x00522BA0);

    if (a2 != g_MissingMetricValue)
    {
        unsigned int existingIndex = ApplyFloatToGlobalObject(
            reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(g_Value_8B0674))),
            nullptr, a2);

        if (existingIndex != static_cast<unsigned int>(-1))
        {
            uint8_t* entries = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0000));
            CSF_AT(uint32_t, entries, 12u * existingIndex + 4u) = static_cast<uint32_t>(a3);
            return a3;
        }
    }

    // Append branch: the tag is read from the global object before the capacity check.
    void* valueObject = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(g_Value_8B0674)));
    uint32_t tag = CSF_AT(uint32_t, valueObject, 0x5C);

    if (self->field_0008 >= self->field_0004)
        GrowSoundEntryBuffer(self, nullptr, static_cast<int>(self->field_000c));

    uint32_t newIndex = self->field_0008;
    uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0000)) + 12u * newIndex;
    self->field_0008 = newIndex + 1;

    CSF_AT(uint32_t, entry, 0) = tag;
    CSF_AT(uint32_t, entry, 4) = static_cast<uint32_t>(a3);
    CSF_AT(float, entry, 8) = a2;

    return static_cast<int>(newIndex + 1);
}

HOOK(0x00523290, SoundManager_AddOrUpdateEntry, "auto");
