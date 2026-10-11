#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Returns the signed byte at +92 of the object pointed to by
// (g_CharacterSelectState8B0694->field_4 - 776), or 0 if any link is null.
static int __cdecl MpSpawn_GetSelectedObjectFlagByte()
{
    int result = 0;
    CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

    if (g_CharacterSelectState8B0694)
    {
        uint8_t* state = reinterpret_cast<uint8_t*>(g_CharacterSelectState8B0694);
        uint32_t innerPtr = CSF_AT(uint32_t, state, 0x4);
        if (innerPtr)
        {
            uint32_t entry = innerPtr - 776;
            if (entry)
            {
                uint8_t* entryBase = reinterpret_cast<uint8_t*>(entry);
                result = static_cast<int>(CSF_AT(int8_t, entryBase, 92));
            }
        }
    }
    return result;
}

HOOK(0x551490, MpSpawn_GetSelectedObjectFlagByte, "auto");
