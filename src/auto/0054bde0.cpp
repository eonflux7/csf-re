#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(unsigned int, __fastcall, InsertRepeatedElements, 0x0054B9F0,
    (void* self, void* edx, int a1, unsigned int a3, uint32_t* arg8));
static CSF_FUNC(uint32_t*, __fastcall, EraseElementRange, 0x007721A0,
    (void* self, void* edx, uint32_t* a2, uint32_t* a3, uint32_t* a4));

static unsigned int __fastcall S_007721a0_Resize(
    S_007721a0* self, void* /*edx*/, unsigned int count, char value, int /*unused*/)
{
    // The packet also labels this direct callee address as a vtable reference.
    (void)CSF_VTBL(0x007721A0);

    const uint32_t begin = self->field_0004;
    unsigned int size = begin
        ? static_cast<int32_t>(self->field_0008 - begin) >> 3
        : 0;

    if (size < count)
    {
        const unsigned int currentSize = begin
            ? static_cast<int32_t>(self->field_0008 - begin) >> 3
            : 0;
        uint32_t valueStorage = static_cast<unsigned char>(value);
        return InsertRepeatedElements(
            self, nullptr, static_cast<int32_t>(self->field_0008),
            count - currentSize, &valueStorage);
    }

    if (begin)
    {
        const uint32_t end = self->field_0008;
        size = static_cast<int32_t>(end - begin) >> 3;
        if (count < size)
        {
            return reinterpret_cast<uint32_t>(EraseElementRange(
                self, nullptr, &count,
                reinterpret_cast<uint32_t*>(begin + 8 * count),
                reinterpret_cast<uint32_t*>(end)));
        }
    }
    return size;
}

HOOK(0x0054BDE0, S_007721a0_Resize, "auto");
