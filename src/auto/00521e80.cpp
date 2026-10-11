// Asks the audio manager for its current object (slot +0x70); when there is one and its slot +0x3C query with 0
// succeeds, forwards the call to that object's slot +0x14, else returns 0 (0x00521E80).
#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

static hook::Tail __fastcall AudioSoundManager_ForwardToCurrentIfQuerySucceeds(void* /*self*/, const uint32_t* /*args*/)
{
    void* manager = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
    void* current = (*reinterpret_cast<void*(__fastcall***)(void*, void*)>(manager))[0x70 / 4](manager, nullptr);
    if (!current)
        return hook::tail_ret(0);
    if (!(*reinterpret_cast<int(__fastcall***)(void*, void*, int)>(current))[0x3C / 4](current, nullptr, 0))
        return hook::tail_ret(0);
    return hook::tail_jump(current, (*reinterpret_cast<void***>(current))[0x14 / 4]);
}

HOOK_TAIL(0x521E80, AudioSoundManager_ForwardToCurrentIfQuerySucceeds, "auto");
