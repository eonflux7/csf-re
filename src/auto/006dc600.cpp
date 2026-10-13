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

CSF_FUNC(int, __cdecl, RwEngineGetNumVideoModes, 0x00626B90, ());
CSF_FUNC(int, __cdecl, RwEngineGetVideoModeInfo, 0x00626BE0, (int a1, int a2));

// Walks every RwEngine video mode, asking the render engine's virtual
// slot 0x70 whether each mode matches. Returns true as soon as one matches.
static bool __fastcall C_00861460_FindVideoModeMatch(C_00861460* self, void* /*edx*/, int a2)
{
    int matchedModeIndex = -1;
    int modeIndex = 0;

    int numVideoModes = RwEngineGetNumVideoModes();
    if (numVideoModes <= 0)
        return false;

    alignas(8) uint8_t videoModeInfoBytes[0x1C];

    while (matchedModeIndex == -1)
    {
        RwEngineGetVideoModeInfo(reinterpret_cast<int>(videoModeInfoBytes), modeIndex);

        bool isMatch = ((bool(__fastcall*)(void*, void*, uint32_t*, int))(CSF_AT(void**, self, 0)[0x70 / 4]))(
            self, nullptr, reinterpret_cast<uint32_t*>(videoModeInfoBytes), a2);
        if (isMatch)
            matchedModeIndex = modeIndex;

        if (++modeIndex >= numVideoModes)
            return matchedModeIndex != -1;
    }
    return true;
}

HOOK(0x006DC600, C_00861460_FindVideoModeMatch, "auto");
