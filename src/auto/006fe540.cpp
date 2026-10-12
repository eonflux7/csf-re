#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));
CSF_FUNC(uint16_t, __fastcall, GetInnerObjectWord12, 0x006FF550, (void* self, void*));

static uint32_t* __fastcall C_00863044_RemoveClump(
    C_00863044* self, void* /*edx*/, uint32_t index)
{
    const uint32_t entryOffset = 12 * index;
    void* object = CSF_AT(void*, reinterpret_cast<void*>(self->field_0024), entryOffset);
    void* innerObject = CSF_AT(void*, object, 4);
    CSF_AT(uint32_t, innerObject, 8) |= 1u;

    object = CSF_AT(void*, reinterpret_cast<void*>(self->field_0024), entryOffset);
    const uint32_t innerCount = GetInnerObjectWord12(object, nullptr);

    for (uint32_t innerIndex = 0; innerIndex < innerCount; ++innerIndex)
    {
        object = CSF_AT(void*, reinterpret_cast<void*>(self->field_0024), entryOffset);
        ((void(__fastcall*)(void*, void*, uint32_t, uint32_t))
            CSF_AT(void**, object, 0)[0x54 / 4])(
                object, nullptr, 0, innerIndex);

        object = CSF_AT(void*, reinterpret_cast<void*>(self->field_0024), entryOffset);
        ((void(__fastcall*)(void*, void*, uint32_t, uint32_t))
            CSF_AT(void**, object, 0)[0x58 / 4])(
                object, nullptr, 0, innerIndex);
    }

    void* globalObject = reinterpret_cast<void*>(GetGlobalInt());
    void* renderHandle = reinterpret_cast<void*>(
        RenderHandle_FromObject(globalObject, nullptr));
    void* clumpManager = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, renderHandle, 0)[0x38 / 4])(renderHandle, nullptr);

    object = CSF_AT(void*, reinterpret_cast<void*>(self->field_0024), entryOffset);
    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, clumpManager, 0)[4 / 4])(
            clumpManager, nullptr, object);

    uint32_t* result = reinterpret_cast<uint32_t*>(index);
    if (index < self->field_002c - 1u)
    {
        void* lastEntry = reinterpret_cast<void*>(
            self->field_0024 + 12 * self->field_002c - 12);
        result = reinterpret_cast<uint32_t*>(self->field_0024 + entryOffset);

        CSF_AT(uint32_t, result, 0) = CSF_AT(uint32_t, lastEntry, 0);
        CSF_AT(uint32_t, result, 4) = CSF_AT(uint32_t, lastEntry, 4);
        CSF_AT(uint32_t, result, 8) = CSF_AT(uint32_t, lastEntry, 8);
    }

    --self->field_002c;
    return result;
}

HOOK(0x006FE540, C_00863044_RemoveClump, "auto");
