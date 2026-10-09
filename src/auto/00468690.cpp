#include <cstdint>
#include "globals.h"
#include "hook.h"

// Original 0x00468660: parses a mission-script entry. Bound as seen from callers.
CSF_FUNC(uint32_t*, __fastcall, MissionScript_ParseEntry, 0x00468660, (void* self, void* edx, char a2));

// 0x00468690: adjusts `this` back by 12 bytes (the sub-object base sits at -12)
// and tail-forwards to the parser with the same stack argument.
static uint32_t* __fastcall MissionScript_ParseEntryViaBase12(char* self, void* /*edx*/, char a2)
{
    return MissionScript_ParseEntry(self - 12, nullptr, a2);
}

HOOK(0x00468690, MissionScript_ParseEntryViaBase12, "auto");
