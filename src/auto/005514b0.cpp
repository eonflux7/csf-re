#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callee: sub_748700, __fastcall with ecx = this (no stack args, ret 0).
CSF_FUNC(int, __fastcall, MpSpawn_ProcessSelectedSlot, 0x00748700, (void* self, void* edx));

CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

// Returns -1 when no character-select state (or no slot object) is available.
// Otherwise forwards the slot object (state+4, minus 776 bytes) to the slot processor.
static int __cdecl MpSpawn_QuerySelectedSpawnState()
{
    int result = -1;

    if (g_CharacterSelectState8B0694)
    {
        uint8_t* state = reinterpret_cast<uint8_t*>(g_CharacterSelectState8B0694);
        uint32_t slotBase = CSF_AT(uint32_t, state, 4);
        if (slotBase)
        {
            uint32_t slotObject = slotBase - 776;
            if (slotObject)
            {
                return MpSpawn_ProcessSelectedSlot(
                    reinterpret_cast<void*>(static_cast<uintptr_t>(slotObject)), nullptr);
            }
        }
    }
    return result;
}

HOOK(0x005514B0, MpSpawn_QuerySelectedSpawnState, "auto");
