#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static char __fastcall CSoundInstance_ApplyPendingVolumeToVoice(
    CSoundInstance* self, void* /*edx*/, int /*a2*/)
{
    self->vSlot20();

    if (self->field_005f == 0)
        return 0;

    void* voice = reinterpret_cast<void*>(self->m_voice);
    int position = static_cast<int>(self->field_0060);
    int voicePosition =
        ((int(__fastcall*)(void*, void*))CSF_AT(void**, voice, 0)[0x34 / 4])(
            voice, nullptr);

    if (position >= voicePosition)
        return static_cast<char>(voicePosition);

    uint32_t savedBits;
    std::memcpy(&savedBits, &self->field_0064, sizeof(savedBits));
    self->vSlot28(position, nullptr);
    std::memcpy(&self->field_0064, &savedBits, sizeof(savedBits));

    return static_cast<char>(savedBits & 0xFF);
}

HOOK(0x00526940, CSoundInstance_ApplyPendingVolumeToVoice, "auto");
