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

// Assigns an empty string range to a string-range sub-object.
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

// Resets the briefing sound slot state: clears the playback index, marks the
// three slot ids as unused, empties all string-range sub-objects, and clears
// the trailing id and flag. Returns the eax value of the last string assignment.
static int __fastcall CampaignBriefing_ResetSlots(char* self, void* /*edx*/)
{
    char* emptyText = reinterpret_cast<char*>(&g_byte_82E65C);

    CSF_AT(int, self, 0xB8) = 0;

    AssignStringRange_429B60(self + 4, nullptr, emptyText, 0);

    CSF_AT(int, self, 0x3C) = -1;
    CSF_AT(int, self, 0x5C) = -1;
    CSF_AT(int, self, 0x7C) = -1;

    AssignStringRange_429B60(self + 0x9C, nullptr, emptyText, 0);
    AssignStringRange_429B60(self + 0x40, nullptr, emptyText, 0);
    AssignStringRange_429B60(self + 0x60, nullptr, emptyText, 0);
    void* lastResult = AssignStringRange_429B60(self + 0x80, nullptr, emptyText, 0);

    CSF_AT(int, self, 0xBC) = -1;
    CSF_AT(int, self, 0x00) = -1;
    CSF_AT(uint8_t, self, 0xC0) = 0;

    return static_cast<int>(reinterpret_cast<intptr_t>(lastResult));
}

HOOK(0x0043FA70, CampaignBriefing_ResetSlots, "auto");
