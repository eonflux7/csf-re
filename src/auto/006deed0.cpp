#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, MoveTextureRecords, 0x006DEC00,
         (int a5, int a2, void* a4));
CSF_FUNC(int, __cdecl, ResetRecordRange, 0x006DEC40,
         (int* begin, int* end, ...));

static int* __fastcall S_006deed0_EraseRecordRange(
    S_006deed0* self,
    void* /*edx*/,
    int* output,
    int rangeBegin,
    int rangeEnd)
{
    if (rangeBegin != rangeEnd)
    {
        int* newEnd = static_cast<int*>(MoveTextureRecords(
            rangeEnd,
            static_cast<int>(self->field_0008),
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(rangeBegin))));

        ResetRecordRange(
            newEnd,
            reinterpret_cast<int*>(static_cast<std::uintptr_t>(self->field_0008)),
            self,
            output);

        self->field_0008 =
            static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(newEnd));
    }

    *output = rangeBegin;
    return output;
}

HOOK(0x006DEED0, S_006deed0_EraseRecordRange, "auto");
