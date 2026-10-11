#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

static uint32_t* __fastcall C_00840f6c_HandleObjectRemovalEvent(
    C_00840f6c* self, void* /*edx*/, void* event)
{
    const uint32_t eventType = CSF_AT(uint32_t, event, 0x04);
    if (eventType != 8 && eventType != 19)
        return reinterpret_cast<uint32_t*>(eventType);

    if (self->field_0018 == 0)
        return nullptr;

    uint32_t index = 0;
    uint32_t entryOffset = 0;
    uint32_t matchedIndex = UINT32_MAX;
    bool found = false;

    while (!found)
    {
        void* entries = reinterpret_cast<void*>(self->field_0010);
        if (CSF_AT(uint32_t, entries, entryOffset + 8) ==
            CSF_AT(uint32_t, event, 0x0C))
        {
            matchedIndex = index;
            found = true;
        }

        ++index;
        entryOffset += 28;
        if (index >= self->field_0018)
        {
            if (!found)
                return reinterpret_cast<uint32_t*>(index);
            break;
        }
    }

    void* global = reinterpret_cast<void*>(GetGlobalInt());
    void* registry = reinterpret_cast<void*>(
        SceneLoader_GetObjectRegistry(global, nullptr));

    const uint32_t matchedOffset = 28 * matchedIndex;
    const uint32_t objectHandle = CSF_AT(
        uint32_t, reinterpret_cast<void*>(self->field_0010), matchedOffset);

    uint32_t* result =
        (reinterpret_cast<uint32_t* (__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, registry, 0)[0x18 / 4]))(
                registry, nullptr, objectHandle);

    if (matchedIndex < self->field_0018 - 1)
    {
        const uint32_t lastOffset = 28 * self->field_0018 - 28;
        void* lastEntry = reinterpret_cast<void*>(
            self->field_0010 + lastOffset);
        const uint32_t lastHandle = CSF_AT(uint32_t, lastEntry, 0);

        result = reinterpret_cast<uint32_t*>(
            self->field_0010 + matchedOffset);
        CSF_AT(uint32_t, result, 0) = lastHandle;
        CSF_AT(uint32_t, result, 8) = CSF_AT(uint32_t, lastEntry, 8);
        CSF_AT(uint32_t, result, 0x0C) = CSF_AT(uint32_t, lastEntry, 0x0C);
        CSF_AT(uint32_t, result, 0x10) = CSF_AT(uint32_t, lastEntry, 0x10);
        CSF_AT(uint32_t, result, 0x14) = CSF_AT(uint32_t, lastEntry, 0x14);
        CSF_AT(uint32_t, result, 0x18) = CSF_AT(uint32_t, lastEntry, 0x18);
    }

    --self->field_0018;
    return result;
}

HOOK(0x0051EC40, C_00840f6c_HandleObjectRemovalEvent, "auto");
