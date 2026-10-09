#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Stops the attached voice (vtable slot +0x8), then resets the instance's
// playback, loop, follow, fade and state fields to their defaults.
static int __fastcall CSoundInstance_ResetPlaybackState(CSoundInstance* self, void* /*edx*/)
{
    void* voice = reinterpret_cast<void*>(self->m_voice);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(CSF_AT(void**, voice, 0)[2])(voice, nullptr);

    self->m_playing = 0;
    self->m_followEntity = 0;
    self->field_005e = 0;
    self->m_loop = 0;

    self->m_fadeTime = 0.0f;
    self->field_004c = 0.0f;

    self->field_0054 = 0xFFFFFFFFu;
    self->field_0058 = 0xFFFFFFFFu;

    self->field_0064 = 0.0f;
    self->field_0068 = 0.0f;
    self->m_state = 0;
    self->field_005c = 0;

    return 0;
}

HOOK(0x005275F0, CSoundInstance_ResetPlaybackState, "auto");
