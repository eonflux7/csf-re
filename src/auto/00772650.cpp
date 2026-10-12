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
#include "legacy/string.h"

static CSF_FUNC(char, __fastcall, AppendRepeatedValues, 0x007721F0,
    (void* self, void* edx, char* a1, unsigned int a3, uint32_t* arg8));
static CSF_FUNC(uint32_t*, __fastcall, EraseElementRange, 0x007721A0,
    (void* self, void* edx, uint32_t* a2, uint32_t* a3, uint32_t* a4));

static char __fastcall S_007721a0_ResizeToCount(
    S_007721a0* self, void* /*edx*/, unsigned int count, int value, int /*unused*/)
{
    // The packet also labels this direct callee address as a vtable reference.
    (void)CSF_VTBL(0x007721A0);

    const uint32_t begin = self->field_0004;
    unsigned int size = begin
        ? static_cast<unsigned int>(static_cast<int32_t>(self->field_0008 - begin) >> 3)
        : 0;

    if (size < count)
    {
        const unsigned int currentSize = begin
            ? static_cast<unsigned int>(static_cast<int32_t>(self->field_0008 - begin) >> 3)
            : 0;
        uint32_t valueStorage = static_cast<uint32_t>(value);
        return static_cast<char>(AppendRepeatedValues(
            self, nullptr, reinterpret_cast<char*>(self->field_0008),
            count - currentSize, &valueStorage));
    }

    if (begin)
    {
        const uint32_t end = self->field_0008;
        size = static_cast<unsigned int>(static_cast<int32_t>(end - begin) >> 3);
        if (count < size)
        {
            EraseElementRange(
                self, nullptr, &count,
                reinterpret_cast<uint32_t*>(begin + 8 * count),
                reinterpret_cast<uint32_t*>(end));
        }
    }
    return static_cast<char>(size);
}

HOOK(0x00772650, S_007721a0_ResizeToCount, "auto");
