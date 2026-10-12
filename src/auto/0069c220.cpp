#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int*, __fastcall, RangeAdvanceFromIndex, 0x0069BD20, (void* self, void* edx, int* a4, int arg4, int arg8));
CSF_FUNC(char, __fastcall, RangeExtendToCount, 0x0069BF30, (void* self, void* edx, char* a1, unsigned int a3, int* arg8));

// Number of elements in the range [field_0004, field_0008), 32 bytes each. Zero when the range is empty.
static uint32_t ElementCountOf(S_0069bd20* self)
{
    uint32_t begin = self->field_0004;
    if (begin == 0)
        return 0;
    int32_t byteSpan = static_cast<int32_t>(self->field_0008 - begin);
    return static_cast<uint32_t>(byteSpan >> 5);
}

static uint8_t __fastcall S_0069bd20_SetElementCount(
    S_0069bd20* self,
    void* /*edx*/,
    unsigned int targetCount,
    int a3,
    int a4,
    void* block,
    int a6,
    int a7,
    int a8,
    int a9,
    unsigned int a10)
{
    uint32_t begin = self->field_0004;
    uint8_t result = static_cast<uint8_t>(targetCount);

    if (ElementCountOf(self) >= targetCount)
    {
        if (begin != 0 && targetCount < ElementCountOf(self))
        {
            (void)CSF_VTBL(0x0069BD20);
            result = static_cast<uint8_t>(reinterpret_cast<uintptr_t>(
                RangeAdvanceFromIndex(
                    self,
                    nullptr,
                    reinterpret_cast<int*>(&targetCount),
                    static_cast<int>(begin + 32u * targetCount),
                    static_cast<int>(self->field_0008))));
        }
    }
    else
    {
        uint32_t currentCount = ElementCountOf(self);
        result = static_cast<uint8_t>(RangeExtendToCount(
            self,
            nullptr,
            reinterpret_cast<char*>(static_cast<uintptr_t>(self->field_0008)),
            targetCount - currentCount,
            &a3));
    }

    if (a10 >= 0x10u)
    {
        result = static_cast<uint8_t>(reinterpret_cast<uintptr_t>(block));
        if (block)
            return static_cast<uint8_t>(Mem_FreeDebug(block));
    }
    return result;
}

HOOK(0x0069C220, S_0069bd20_SetElementCount, "auto");
