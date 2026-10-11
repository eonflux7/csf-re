#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(bool, __fastcall, sub_4285B0, 0x004285B0, (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, MenuControls_QueryFlag, 0x004285D0, (void* self, void* edx, int value));
CSF_FUNC(bool, __fastcall, sub_4285F0, 0x004285F0, (void* self, void* edx, int a2));

// Advances the cheat key-sequence cursor. Checks whether the next key in the
// sequence matches; on a match moves the cursor forward and, once the end of
// the sequence is reached, marks the global flag at +0x50. On a miss, checks
// the previous key and resets the cursor to 0 if that also fails.
static char __fastcall Cheats_AdvanceSequenceCursor(void* self, void* /*edx*/)
{
    // Low byte of the last value left in al by the original (tracked explicitly).
    uint8_t lowByte = CSF_AT(uint8_t, self, 0xF4);
    if (lowByte == 0)
        return static_cast<char>(lowByte);

    void* gameObj = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    lowByte = static_cast<uint8_t>(reinterpret_cast<uintptr_t>(gameObj));
    if (CSF_AT(uint8_t, gameObj, 0x50) != 0)
        return static_cast<char>(lowByte);

    void* menuOwner = CSF_AT(void*, self, 0x14);
    uint32_t* sequence = CSF_AT(uint32_t*, self, 0xE4);
    int index = CSF_AT(int, self, 0x24);

    if (sub_4285F0(menuOwner, nullptr, static_cast<int>(sequence[index])))
    {
        uint32_t limit = CSF_AT(uint32_t, self, 0xEC);
        uint32_t newIndex = static_cast<uint32_t>(CSF_AT(int, self, 0x24)) + 1;
        CSF_AT(uint32_t, self, 0x24) = newIndex;
        lowByte = static_cast<uint8_t>(newIndex);
        if (limit <= newIndex)
        {
            gameObj = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            lowByte = static_cast<uint8_t>(reinterpret_cast<uintptr_t>(gameObj));
            CSF_AT(uint8_t, gameObj, 0x50) = 1;
        }
        return static_cast<char>(lowByte);
    }

    // Miss: test the current key against the alternate handler.
    menuOwner = CSF_AT(void*, self, 0x14);
    sequence = CSF_AT(uint32_t*, self, 0xE4);
    index = CSF_AT(int, self, 0x24);
    bool altMatched = sub_4285B0(menuOwner, nullptr, static_cast<int>(sequence[index]));
    lowByte = altMatched ? 1 : 0;
    if (altMatched)
        return static_cast<char>(lowByte);

    uint32_t cursor = CSF_AT(uint32_t, self, 0x24);
    lowByte = static_cast<uint8_t>(cursor);
    if (cursor != 0)
    {
        sequence = CSF_AT(uint32_t*, self, 0xE4);
        bool prevMatched = MenuControls_QueryFlag(
            CSF_AT(void*, self, 0x14), nullptr, static_cast<int>(sequence[cursor - 1]));
        lowByte = prevMatched ? 1 : 0;
        if (prevMatched)
            return static_cast<char>(lowByte);
    }

    CSF_AT(int, self, 0x24) = 0;
    return static_cast<char>(lowByte);
}

HOOK(0x0042C540, Cheats_AdvanceSequenceCursor, "auto");
