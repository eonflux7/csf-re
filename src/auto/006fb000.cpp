#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __cdecl, GfxScreenEffects_ShiftTimedEntriesLeft, 0x006F9FE0, (std::uint32_t* first, std::uint32_t* last, std::uint32_t* destination, int));
CSF_FUNC(int, __fastcall, GfxScreenEffects_InsertTimedEntries, 0x006FAD30, (void* self, void* edx, char* a1, unsigned int a3, uint32_t* arg8));

static std::uint32_t __fastcall S_004482c0_ResizeTimedEntries(
    S_004482c0* self, void* /*edx*/, unsigned int requestedCount,
    int entryWord0, int entryWord1, int entryWord2,
    int entryWord3, int entryWord4)
{
    const std::uint32_t begin = self->field_0004;
    std::uint32_t result = begin
        ? static_cast<std::int32_t>(self->field_0008 - begin) / 20
        : 0;

    if (result < requestedCount)
    {
        const int currentCount = begin
            ? static_cast<std::int32_t>(self->field_0008 - begin) / 20
            : 0;
        char* insertionPoint = reinterpret_cast<char*>(self->field_0008);
        std::uint32_t entry[5] = {
            static_cast<std::uint32_t>(entryWord0),
            static_cast<std::uint32_t>(entryWord1),
            static_cast<std::uint32_t>(entryWord2),
            static_cast<std::uint32_t>(entryWord3),
            static_cast<std::uint32_t>(entryWord4)
        };
        return GfxScreenEffects_InsertTimedEntries(
            self, nullptr, insertionPoint, requestedCount - currentCount, entry);
    }

    if (begin)
    {
        const std::uint32_t end = self->field_0008;
        result = static_cast<std::int32_t>(end - begin) / 20;
        if (requestedCount < result)
        {
            result = begin + 20 * requestedCount;
            if (result != end)
            {
                result = reinterpret_cast<std::uint32_t>(
                    GfxScreenEffects_ShiftTimedEntriesLeft(
                        reinterpret_cast<std::uint32_t*>(end),
                        reinterpret_cast<std::uint32_t*>(end),
                        reinterpret_cast<std::uint32_t*>(result),
                        static_cast<int>(requestedCount)));
                self->field_0008 = result;
            }
        }
    }

    return result;
}

HOOK(0x006FB000, S_004482c0_ResizeTimedEntries, "auto");
