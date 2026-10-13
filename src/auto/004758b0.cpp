#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));

CSF_GLOBAL(void**, g_SceneEntryObjects, 0x00899744);
CSF_GLOBAL(uint32_t, g_SceneEntryCount, 0x0089974C);

static uint32_t __fastcall DispatchRecordOrQueueIdentifier(
    void* self, void* /*edx*/, void* record, char dispatchImmediately)
{
    if (dispatchImmediately)
    {
        for (uint32_t index = 0; index < CSF_AT(uint32_t, self, 0x30); ++index)
        {
            void* recipient = CSF_AT(void**, self, 0x28)[index];
            reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, recipient, 0)[0])(recipient, nullptr, record);
        }

        uint32_t result = g_SceneEntryCount;
        for (uint32_t index = 0; index < result; ++index)
        {
            void* recipient = g_SceneEntryObjects[index];
            reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, recipient, 0)[0])(recipient, nullptr, record);
            result = g_SceneEntryCount;
        }
        return result;
    }

    S_0041e5f0* identifiers;
    if (CSF_AT(uint8_t, self, 0x24))
        identifiers = reinterpret_cast<S_0041e5f0*>(
            reinterpret_cast<uint8_t*>(self) + 0x14);
    else
        identifiers = reinterpret_cast<S_0041e5f0*>(
            reinterpret_cast<uint8_t*>(self) + 4);

    const int identifier =
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, record, 0)[0x1C / 4])(record, nullptr);

    if (static_cast<uint32_t>(identifiers->field_0008) >=
        static_cast<uint32_t>(identifiers->field_0004))
    {
        S_0041e5f0_GrowArrayCapacity(
            identifiers, nullptr, identifiers->field_000c);
    }

    const uint32_t index = identifiers->field_0008;
    int* destination =
        reinterpret_cast<int*>(identifiers->field_0000) + index;
    const uint32_t result = index + 1;
    identifiers->field_0008 = result;
    *destination = identifier;
    return result;
}

HOOK(0x004758B0, DispatchRecordOrQueueIdentifier, "auto");
