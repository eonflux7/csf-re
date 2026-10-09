#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());
CSF_GLOBAL(float, g_ListenerOffsetScale_83BB14, 0x0083BB14);

// Adds a listener-relative offset (scaled by a constant) to the voice's
// position vector and forwards it to the voice device object.
static int __fastcall CSoundInstance_UpdateVoicePositionFromListener(CSoundInstance* self, void* /*edx*/)
{
    float offsetVector[3];
    float voicePosition[3];

    void* globalObjA = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_70DDE0()));
    ((void(__fastcall*)(void*, void*, float*))CSF_AT(void**, globalObjA, 0)[0x4C / 4])(globalObjA, nullptr, offsetVector);

    void* globalObjB = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_70DDE0()));
    ((void(__fastcall*)(void*, void*, float*))CSF_AT(void**, globalObjB, 0)[0x44 / 4])(globalObjB, nullptr, voicePosition);

    float scale = g_ListenerOffsetScale_83BB14;
    voicePosition[0] = offsetVector[0] * scale + voicePosition[0];
    voicePosition[1] = voicePosition[1] + offsetVector[1] * scale;
    voicePosition[2] = voicePosition[2] + offsetVector[2] * scale;

    void* voice = reinterpret_cast<void*>(static_cast<uintptr_t>(self->m_voice));
    return ((int(__fastcall*)(void*, void*, float*))CSF_AT(void**, voice, 0)[0x1C / 4])(voice, nullptr, voicePosition);
}

HOOK(0x005268C0, CSoundInstance_UpdateVoicePositionFromListener, "auto");
