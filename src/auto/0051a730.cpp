#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, sub_51A180, 0x0051A180, (int a2, int arg4, void* a3));
CSF_FUNC(int, __cdecl, GfxTextureLoader_ResetRecordRange, 0x006DEC40, (int* begin, int* end));

static int* __fastcall SetRecordRangeAndWriteValue(
    S_0051a730* self,
    void* /*edx*/,
    int* outputSlot,
    int newValue,
    int previousValue)
{
    if (newValue != previousValue)
    {
        void* newRecord = sub_51A180(
            previousValue,
            static_cast<int>(self->field_0008),
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(newValue)));

        GfxTextureLoader_ResetRecordRange(
            reinterpret_cast<int*>(newRecord),
            reinterpret_cast<int*>(static_cast<std::uintptr_t>(self->field_0008)));

        self->field_0008 = static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(newRecord));
    }

    *outputSlot = newValue;
    return outputSlot;
}

HOOK(0x0051A730, SetRecordRangeAndWriteValue, "auto");
