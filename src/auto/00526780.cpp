#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Stops the sound's voice through the audio device's vtable slot 0x28 (slot 10).
static void __fastcall CSoundInstance_ProcessPendingStop(CSoundInstance* self, void* /*edx*/)
{
    if (self->m_loop)
    {
        // Looping sound was requested to end: clear the loop flag and stop the voice.
        void* voice = reinterpret_cast<void*>(self->m_voice);
        self->m_loop = 0;
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, voice, 0)[0x28 / 4])(voice, nullptr);
        self->field_004c = 0.0f;
    }
    else if (!(self->field_0064 > g_fZero))
    {
        // Fade value is not positive (also taken for NaN, matching the original fcomp/jne test):
        // stop the voice immediately.
        void* voice = reinterpret_cast<void*>(self->m_voice);
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, voice, 0)[0x28 / 4])(voice, nullptr);
        self->field_004c = 0.0f;
    }
    else
    {
        // Positive fade value: enter state 2 and latch the fade value.
        self->m_state = 2;
        self->field_0068 = self->field_0064;
    }
}

HOOK(0x00526780, CSoundInstance_ProcessPendingStop, "auto");
