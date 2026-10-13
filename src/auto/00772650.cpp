#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(uint32_t*, __fastcall, S_007721a0_EraseElementRange, 0x007721A0,
    (S_007721a0* self, void* edx, uint32_t* outIterator, uint32_t* eraseFirst, uint32_t* moveFrom));
static CSF_FUNC(char, __fastcall, AppendRepeatedValues, 0x007721F0,
    (void* self, void* edx, char* a1, unsigned int a3, uint32_t* arg8));

static char __fastcall S_007721a0_ResizeToCount(
    S_007721a0* self, void* /*edx*/, unsigned int count, int a3, int a4)
{
    // The packet labels this direct callee address as a vtable reference.
    (void)CSF_VTBL(0x007721A0);
    (void)a4;

    const uint32_t begin = self->field_0004;
    unsigned int size = begin
        ? static_cast<unsigned int>(static_cast<int32_t>(self->field_0008 - begin) >> 3)
        : 0u;

    if (size >= count)
    {
        if (self->field_0004)
        {
            size = static_cast<unsigned int>(static_cast<int32_t>(self->field_0008 - self->field_0004) >> 3);
            if (count < size)
            {
                const uint32_t eraseResult = reinterpret_cast<uint32_t>(
                    S_007721a0_EraseElementRange(
                        self, nullptr,
                        reinterpret_cast<uint32_t*>(&count),
                        reinterpret_cast<uint32_t*>(self->field_0004 + 8 * count),
                        reinterpret_cast<uint32_t*>(self->field_0008)));
                return static_cast<char>(eraseResult);
            }
        }
        return static_cast<char>(size);
    }

    unsigned int currentSize = self->field_0004
        ? static_cast<unsigned int>(static_cast<int32_t>(self->field_0008 - self->field_0004) >> 3)
        : 0u;

    return AppendRepeatedValues(
        self, nullptr,
        reinterpret_cast<char*>(self->field_0008),
        count - currentSize,
        reinterpret_cast<uint32_t*>(&a3));
}

HOOK(0x00772650, S_007721a0_ResizeToCount, "auto");
