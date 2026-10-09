#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

static uint32_t* __fastcall CSoundInstance_Construct(
    CSoundInstance* self, void* /*edx*/)
{
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_002c = -1;
    self->field_0030 = -1;
    self->field_0028 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    self->m_attachMode = 1;
    self->field_0008 = 1;
    self->m_fadeInTime = 0.0f;
    self->field_0018 = 0.0f;
    self->field_0038 = 0;
    self->field_0039 = 1;
    self->field_003a = 1;
    self->m_soundId = -1;
    self->m_variant = -1;
    self->m_volume = 0.0f;
    self->m_playing = 0;
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008410B0);
    self->m_entityHandle = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    self->field_0054 = -1;
    self->field_0058 = -1;
    self->field_005f = 0;

    void* audioDevice = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
    uint32_t voice = reinterpret_cast<uint32_t (__fastcall*)(void*, void*, int, int)>(
        CSF_AT(void**, audioDevice, 0)[0x34 / 4])(audioDevice, nullptr, 0, 0);
    self->m_voice = voice;

    void* voiceObject = reinterpret_cast<void*>(voice);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, voiceObject, 0)[0x8 / 4])(voiceObject, nullptr);

    self->m_playing = 0;
    self->m_followEntity = 0;
    self->field_005e = 0;
    self->m_loop = 0;
    self->m_fadeTime = 0.0f;
    self->field_004c = 0.0f;
    self->field_0054 = -1;
    self->field_0058 = -1;
    self->field_0064 = 0.0f;
    self->field_0068 = 0.0f;
    self->m_state = 0;
    self->field_005c = 0;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00527DC0, CSoundInstance_Construct, "auto");
