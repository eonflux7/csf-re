#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

CSF_FUNC(char*, __fastcall, sub_6B29A0, 0x006B29A0, (void* self, void* edx));

// Returns the game-mode index byte stored in the selected mode object, or -1 when
// the character-select state or its mode object is missing.
static int __cdecl GetMpGameModeIndex()
{
    const uint32_t selectState = g_CharacterSelectState8B0694;
    if (selectState != 0)
    {
        auto* state = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(selectState));
        const uint32_t modeAddress = CSF_AT(uint32_t, state, 4);
        if (modeAddress != 0 && modeAddress != 776)
        {
            auto* modeObject = reinterpret_cast<C_00860288*>(static_cast<uintptr_t>(modeAddress - 776));
            char* modeData = sub_6B29A0(modeObject, nullptr);
            return static_cast<int>(static_cast<int8_t>(modeData[6]));
        }
    }
    return -1;
}

HOOK(0x005515F0, GetMpGameModeIndex, "auto");
