#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Stores the low byte of the level on the sound instance, then forwards the
// full value to the voice object's vtable slot 0x30 (slot 12).
static int __fastcall CSoundInstance_SetLevelByte(CSoundInstance* self, void* /*edx*/, int a2)
{
    self->field_005c = static_cast<uint8_t>(a2);

    void* voice = reinterpret_cast<void*>(static_cast<uintptr_t>(self->m_voice));
    using VoiceSetFn = int(__fastcall*)(void*, void*, int);
    VoiceSetFn setter = reinterpret_cast<VoiceSetFn>(CSF_AT(void**, voice, 0)[0x30 / 4]);
    return setter(voice, nullptr, a2);
}

HOOK(0x005267D0, CSoundInstance_SetLevelByte, "auto");
