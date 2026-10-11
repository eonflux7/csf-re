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

CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

// Returns the match-button state from the character-select subobject's vtable slot 2.
// The original tail-jumps through that slot, so the result comes from the target.
static hook::Tail __fastcall MenuMpPause_GetMatchButtonState(void* /*self*/, const uint32_t* /*args*/)
{
    uint32_t selectState = g_CharacterSelectState8B0694;
    if (selectState == 0)
        return hook::tail_ret(0);

    uint32_t selectSubState = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(selectState)), 4);
    if (selectSubState == 0)
        return hook::tail_ret(0);

    uint32_t matchDataAddr = selectSubState - 776;
    if (matchDataAddr == 0)
        return hook::tail_ret(0);

    void* obj = reinterpret_cast<void*>(static_cast<uintptr_t>(matchDataAddr + 0x308));
    return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x08 / 4]);
}

HOOK_TAIL(0x00551B70, MenuMpPause_GetMatchButtonState, "mp_spawn_points");
