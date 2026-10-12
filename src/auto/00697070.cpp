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

CSF_FUNC(int*, __fastcall, EraseWideStringRange, 0x004F21D0, (legacy::wstring* text, void* edx, int* result, std::uint32_t first, std::uint32_t last));
CSF_FUNC(void*, __fastcall, InsertWideCharacters, 0x006959F0, (void* self, void* edx, unsigned int a2, unsigned int a3, int16_t a4));

// Clears the wide string held by `self`, then appends each byte of the
// narrow C string `text` as one 16-bit character. Returns the terminating
// byte value (0) as the original does.
static char __fastcall S_004f21d0_AssignFromNarrowString(S_004f21d0* self, void* /*edx*/, char* text)
{
    int eraseResult = 0;

    // Current data begin / end (inline buffer when capacity < 8, otherwise heap pointer).
    std::uint32_t capacity = self->field_0018;
    uint16_t* inlineBuffer = reinterpret_cast<uint16_t*>(&self->field_0004);
    uint16_t* dataBegin = (capacity < 8)
        ? inlineBuffer
        : *reinterpret_cast<uint16_t**>(&self->field_0004);
    uint16_t* dataEnd = dataBegin + self->field_0014;

    std::uint32_t firstAddr = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(dataBegin));
    std::uint32_t lastAddr = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(dataEnd));
    EraseWideStringRange(reinterpret_cast<legacy::wstring*>(self), nullptr, &eraseResult, firstAddr, lastAddr);

    char* cursor = text;
    char result = *cursor;
    while (result != 0)
    {
        std::uint32_t capacityNow = self->field_0018;
        int16_t unit = static_cast<int16_t>(result);

        uint16_t* beginNow = (capacityNow < 8)
            ? reinterpret_cast<uint16_t*>(&self->field_0004)
            : *reinterpret_cast<uint16_t**>(&self->field_0004);
        uint16_t* endNow = beginNow + self->field_0014;

        std::uintptr_t endAddr = reinterpret_cast<std::uintptr_t>(endNow);
        std::uintptr_t beginAddr = reinterpret_cast<std::uintptr_t>(beginNow);

        std::uint32_t insertIndex = static_cast<std::uint32_t>(endAddr);
        if (insertIndex != 0)
        {
            std::int32_t byteDelta = static_cast<std::int32_t>(static_cast<std::uint32_t>(endAddr) - static_cast<std::uint32_t>(beginAddr));
            insertIndex = static_cast<std::uint32_t>(byteDelta >> 1);
        }

        InsertWideCharacters(reinterpret_cast<void*>(self), nullptr, insertIndex, 1u, unit);

        result = cursor[1];
        ++cursor;
    }
    return result;
}

HOOK(0x697070, S_004f21d0_AssignFromNarrowString, "auto");
