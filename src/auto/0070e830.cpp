#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint8_t*, __cdecl, SceneLoader_CreateLoadTimer, 0x0069CDC0, ());
CSF_FUNC(void, __fastcall, S_0070e170_ProcessPlaybackRequests, 0x0070E220, (void* self, void* edx));
CSF_FUNC(char, __fastcall, S_0070e170_UpdateFadeAndPlayback, 0x0070E3B0, (S_0070e170* self, void* , float elapsed));
CSF_FUNC(char, __fastcall, S_0070e170_FillPcmBuffer, 0x0070E4A0, (S_0070e170* self, void*));
CSF_FUNC(char, __fastcall, S_0070e170_UpdateAudioBuffer, 0x0070E550, (S_0070e170* self, void* , char clearBuffer));
CSF_FUNC(int, __cdecl, ov_pcm_seek_lap, 0x0081DD2E, (void* stream, int offsetLow, int offsetHigh));

using SleepFunction = void (__stdcall*)(uint32_t milliseconds);
CSF_GLOBAL(SleepFunction, g_SleepImportSlot, 0x0082D05C);

static int __fastcall S_0070e170_RunPlaybackLoop(
    S_0070e170* self, void* /*edx*/)
{
    bool reachedEnd = false;
    uint8_t* timer = SceneLoader_CreateLoadTimer();
    float previousTime = static_cast<float>(
        reinterpret_cast<double (__fastcall*)(void*, void*)>(
            CSF_AT(void**, timer, 0)[0x10 / 4])(timer, nullptr));

    while ((!reachedEnd || self->field_0038) && !self->field_0040)
    {
        double currentTime =
            reinterpret_cast<double (__fastcall*)(void*, void*)>(
                CSF_AT(void**, timer, 0)[0x10 / 4])(timer, nullptr);
        float elapsed = static_cast<float>(currentTime - previousTime);
        previousTime = static_cast<float>(currentTime);

        S_0070e170_UpdateFadeAndPlayback(self, nullptr, elapsed);
        S_0070e170_ProcessPlaybackRequests(self, nullptr);

        if (self->field_0038)
        {
            if (!S_0070e170_UpdateAudioBuffer(
                    self, nullptr, static_cast<char>(reachedEnd)))
            {
                g_SleepImportSlot(15);
            }
        }
        else if (self->field_003a)
        {
            g_SleepImportSlot(15);
        }
        else
        {
            reachedEnd = S_0070e170_FillPcmBuffer(self, nullptr) == 0;
            if (reachedEnd && self->field_003b)
            {
                ov_pcm_seek_lap(
                    reinterpret_cast<void*>(self->field_003c), 0, 0);
                reachedEnd = false;
            }
        }
    }

    return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, timer, 0)[0])(timer, nullptr, 1);
}

HOOK(0x0070E830, S_0070e170_RunPlaybackLoop, "auto");
