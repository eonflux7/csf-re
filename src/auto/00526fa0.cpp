#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CSoundInstance_UpdateAttachMode4, 0x005268C0, (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Per-frame update of a playing sound instance: lifetime countdown, looping restart,
// following the attached entity / listener position, and volume fade in/out.
static void __fastcall CSoundInstance_Update(CSoundInstance* self, void* /*edx*/, float deltaTime)
{
    if (self->field_005c)
        return;

    // Lifetime countdown
    if (self->m_fadeTime > g_fZero && self->field_004c > g_fZero)
    {
        self->field_004c = self->field_004c - deltaTime;
        if (self->field_004c <= g_fZero)
            self->vSlot2C();
    }

    void* voice = reinterpret_cast<void*>(self->m_voice);
    bool isPlaying = ((bool(__fastcall*)(void*, void*))CSF_AT(void**, voice, 0)[0x20 / 4])(voice, nullptr);

    bool updatePosition = true;
    if (self->m_loop)
    {
        if (!isPlaying)
        {
            voice = reinterpret_cast<void*>(self->m_voice);
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, voice, 0)[0x24 / 4])(voice, nullptr, 0);
        }
    }
    else if (!isPlaying)
    {
        updatePosition = false;
    }

    if (updatePosition)
    {
        if (self->m_attachMode == 4)
        {
            CSoundInstance_UpdateAttachMode4(self, nullptr);
        }
        else if (self->m_followEntity)
        {
            if (static_cast<int32_t>(self->field_0058) >= 0)
            {
                int entity = CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                                  reinterpret_cast<int>(&self->m_entityHandle));
                if (entity)
                {
                    void** voiceVtbl = CSF_AT(void**, reinterpret_cast<void*>(self->m_voice), 0);
                    int position = GetObjectPositionVector(reinterpret_cast<void*>(entity), nullptr);
                    ((void(__fastcall*)(void*, void*, int))voiceVtbl[0x1C / 4])(
                        reinterpret_cast<void*>(self->m_voice), nullptr, position);
                }
                else
                {
                    voice = reinterpret_cast<void*>(self->m_voice);
                    ((void(__fastcall*)(void*, void*))CSF_AT(void**, voice, 0)[0x28 / 4])(voice, nullptr);
                    self->field_004c = 0.0f;
                }
            }
        }
        else if (self->field_005e)
        {
            void* listener = reinterpret_cast<void*>(GetSceneMenuContext(g_pCurrentScene, nullptr));
            if (listener)
            {
                float positionBuffer[3] = { 0.0f, 0.0f, 0.0f };
                void** voiceVtbl = CSF_AT(void**, reinterpret_cast<void*>(self->m_voice), 0);
                int position = ((int(__fastcall*)(void*, void*, float*))CSF_AT(void**, listener, 0)[0xA4 / 4])(
                    listener, nullptr, positionBuffer);
                ((void(__fastcall*)(void*, void*, int))voiceVtbl[0x1C / 4])(
                    reinterpret_cast<void*>(self->m_voice), nullptr, position);
            }
        }
    }

    // Volume fade
    if (!(self->field_0064 > g_fZero))
        return;

    if (self->m_state == 1)
    {
        // fade in
        voice = reinterpret_cast<void*>(self->m_voice);
        if (self->field_0068 > g_fZero)
        {
            float volume = (g_TransitionOne - self->field_0068 / self->field_0064) * self->m_volume;
            ((void(__fastcall*)(void*, void*, float))CSF_AT(void**, voice, 0)[0x54 / 4])(voice, nullptr, volume);
            self->field_0068 = self->field_0068 - deltaTime;
        }
        else
        {
            float volume = self->m_volume;
            self->m_state = 0;
            ((void(__fastcall*)(void*, void*, float))CSF_AT(void**, voice, 0)[0x54 / 4])(voice, nullptr, volume);
        }
    }
    else if (self->m_state == 2)
    {
        // fade out
        voice = reinterpret_cast<void*>(self->m_voice);
        if (self->field_0068 > g_fZero)
        {
            float volume = (self->field_0068 / self->field_0064) * self->m_volume;
            ((void(__fastcall*)(void*, void*, float))CSF_AT(void**, voice, 0)[0x54 / 4])(voice, nullptr, volume);
            self->field_0068 = self->field_0068 - deltaTime;
        }
        else
        {
            ((void(__fastcall*)(void*, void*))CSF_AT(void**, voice, 0)[0x28 / 4])(voice, nullptr);
            self->field_004c = 0.0f;
            self->m_state = 0;
            self->field_0064 = 0.0f;
        }
    }
}

HOOK(0x00526FA0, CSoundInstance_Update, "auto");
