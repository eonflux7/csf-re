#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));

static void* __fastcall CTextDatabase_SetTextForMask(
    CTextDatabase* self, void* /*edx*/, char* text, unsigned int mask)
{
    int index = -1;
    if (mask)
    {
        index = 0;
        for (unsigned int remaining = mask >> 1; remaining; remaining >>= 1)
            ++index;
    }

    const std::ptrdiff_t entryOffset = 32 * index;
    auto* stringObject =
        reinterpret_cast<std::uint8_t*>(self->field_0008) + entryOffset + 4;
    unsigned int length = static_cast<unsigned int>(std::strlen(text));

    if (CompareStringSubstring(
            stringObject, nullptr, 0,
            CSF_AT(unsigned int, stringObject, 0x14), text, length) == 0)
    {
        return nullptr;
    }

    auto* entries = reinterpret_cast<std::uint8_t*>(self->field_0008);
    if (CSF_AT(void*, entries + entryOffset, 0))
    {
        void* cachedObject = CSF_AT(void*, entries + entryOffset, 0);
        if (cachedObject)
        {
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, cachedObject, 0)[0])(
                    cachedObject, nullptr, 1);
        }

        CSF_AT(void*,
               reinterpret_cast<std::uint8_t*>(self->field_0008) + entryOffset,
               0) = nullptr;
    }

    length = static_cast<unsigned int>(std::strlen(text));
    return AssignStringRange_429B60(
        reinterpret_cast<std::uint8_t*>(self->field_0008) + entryOffset + 4,
        nullptr, text, length);
}

HOOK(0x0069BB10, CTextDatabase_SetTextForMask, "auto");
