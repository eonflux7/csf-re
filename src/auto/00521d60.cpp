// Stores whether the float argument is positive at this+0xC, then forwards the call to the audio manager's
// vtable slot +0x8 (0x00521D60).
#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static hook::Tail __fastcall AudioSoundManager_SetPositiveFlagAndForward(void* self, const uint32_t* args)
{
    float value;
    std::memcpy(&value, &args[0], sizeof(value));
    CSF_AT(uint8_t, self, 0xC) = value > g_fZero;

    void* manager = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
    return hook::tail_jump(manager, (*reinterpret_cast<void***>(manager))[0x8 / 4]);
}

HOOK_TAIL(0x521D60, AudioSoundManager_SetPositiveFlagAndForward, "auto");
