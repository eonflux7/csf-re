#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, MenuAccount_GateCheck_696B60, 0x00696B60, (void* self, void* edx, void* a2));
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static char __fastcall InsertPositionIntoRanges(void* self, void* /*edx*/, unsigned int position)
{
    char result = static_cast<char>(CSF_AT(uint8_t, self, 0xDA));
    if (!result)
        return result;

    result = static_cast<char>(CSF_AT(uint8_t, self, 0xD9));
    if (!result)
        return result;

    result = static_cast<char>(MenuAccount_GateCheck_696B60(
        static_cast<uint8_t*>(self) + 0x3C, nullptr, &g_byte_82E65C));
    if (result)
        return result;

    const uint32_t count = CSF_AT(uint32_t, self, 0xE4);
    for (uint32_t index = 0; index < count; ++index)
    {
        auto* range = CSF_AT(uint8_t*, self, 0xDC) + 8 * index;
        const uint32_t start = CSF_AT(uint32_t, range, 0);
        const uint32_t end = CSF_AT(uint32_t, range, 4) + start - 1;
        result = static_cast<char>(start);

        if (position <= start)
        {
            auto* startField = CSF_AT(uint8_t*, self, 0xDC) + 8 * index;
            ++CSF_AT(uint32_t, startField, 0);
            result = static_cast<char>(reinterpret_cast<uintptr_t>(startField));
        }
        else if (position < end)
        {
            auto* lengthField = CSF_AT(uint8_t*, self, 0xDC) + 8 * index + 4;
            ++CSF_AT(uint32_t, lengthField, 0);
            result = static_cast<char>(reinterpret_cast<uintptr_t>(lengthField));
        }
    }
    return result;
}

HOOK(0x00733630, InsertPositionIntoRanges, "auto");
