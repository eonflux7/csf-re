#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x82D4CC);

// Returns true while the sound instance must stay alive: a positive
// field_004c, a looping instance whose fade time is zero, or a voice that is
// still active (vslot 0x10) and busy (vslot 0x20) or has field_005c set.
static bool __fastcall CSoundInstance_ShouldKeepAlive(CSoundInstance* self, void* /*edx*/)
{
    if (self->field_004c > g_fZero)
        return true;

    if (self->m_loop != 0 && self->m_fadeTime == g_fZero)
        return true;

    void* voice = reinterpret_cast<void*>(static_cast<uintptr_t>(self->m_voice));

    // voice->vtbl[0x10 / 4]: voice is still open
    bool voiceOpen = reinterpret_cast<bool(__fastcall*)(void*, void*)>(
        CSF_AT(void**, voice, 0)[0x10 / 4])(voice, nullptr);
    if (!voiceOpen)
        return false;

    // voice->vtbl[0x20 / 4]: voice is still playing
    bool voicePlaying = reinterpret_cast<bool(__fastcall*)(void*, void*)>(
        CSF_AT(void**, voice, 0)[0x20 / 4])(voice, nullptr);
    if (voicePlaying)
        return true;

    return self->field_005c != 0;
}

HOOK(0x5267F0, CSoundInstance_ShouldKeepAlive, "auto");
