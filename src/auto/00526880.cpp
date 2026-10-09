#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns 1 when the instance is no longer playing; otherwise 1 only when the
// bound voice reports inactive and the 0x5C flag is clear.
static int __fastcall CSoundInstance_IsVoiceFinished(CSoundInstance* self, void* /*edx*/) {
    if (!self->m_playing)
        return 1;
    if (!self->field_003a)
        return 0;

    void* voice = reinterpret_cast<void*>(static_cast<uintptr_t>(self->m_voice));
    using VoiceStateProbe = uint8_t(__fastcall*)(void*, void*);
    // vtable slot 0x20 / 4 == 8
    auto probe = reinterpret_cast<VoiceStateProbe>(CSF_AT(void**, voice, 0)[8]);

    if (probe(voice, nullptr) == 0 && self->field_005c == 0)
        return 1;
    return 0;
}

HOOK(0x00526880, CSoundInstance_IsVoiceFinished, "auto");
