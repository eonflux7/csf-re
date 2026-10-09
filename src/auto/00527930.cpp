#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Resets the playback state of a C_00841128 sound instance and stops its voice.
static int __fastcall C_00841128_ResetPlayback(C_00841128* self, void* /*edx*/) {
    // Stop the voice: vtbl slot +0x8 on the voice object, no arguments.
    void* voice = reinterpret_cast<void*>(self->m_voice);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, voice, 0)[8 / 4])(voice, nullptr);

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
    self->field_0078 = 0;
    return 0;
}

HOOK(0x527930, C_00841128_ResetPlayback, "auto");
