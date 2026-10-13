#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, CoreVariantValues_InitializeFileSystems, 0x006971B0, (int context));
CSF_FUNC(int, __fastcall, C_00863ba8_CreateAndRegisterEffect, 0x0070F0F0, (C_00863ba8* self, void*));
CSF_FUNC(int, __fastcall, C_00863ba8_InitializeEffectState, 0x0070F160, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00863ba8_FinalizeEffectState, 0x0070F1D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, DSound_MapSpeakerConfiguration, 0x007108B0, (C_00863ba8* self, void*));
CSF_FUNC(int, __cdecl, Audio_OpenEngine, 0x007DB6F0, (int a1, int a2));
CSF_FUNC(int, __cdecl, _rxPipelineOpen, 0x007DB920, ());
CSF_FUNC(uint32_t*, __cdecl, Audio_CreateEffectDictionary, 0x007DBBA0, (int a1));
CSF_FUNC(int, __cdecl, RpMTEffectDictSetCurrent, 0x007DBE80, (int a1, int a2));
CSF_FUNC(int, __cdecl, AudioEffect_ReleaseHandle_7DEF60, 0x007DEF60, (void* handle, void* callback, int flags));
CSF_FUNC(int, __cdecl, Audio_FindEffectHandle, 0x007E1420,
         (int* a1, char* a2, int a3, int (__cdecl* a4)(int, int, char*, int), int a5));
CSF_FUNC(int, __cdecl, Audio_InitializeEffectDictionarySupport, 0x007F17C0, ());

CSF_GLOBAL(int, g_AudioEngineHandle, 0x008AC218);

static char __fastcall C_00863ba8_InitializeAudio(
    C_00863ba8* self, void* /*edx*/, int configuration)
{
    const int engineHandle = Audio_OpenEngine(0, 1);
    char success = engineHandle != 0;
    g_AudioEngineHandle = engineHandle;

    if (success)
    {
        _rxPipelineOpen();
        Audio_InitializeEffectDictionarySupport();
        success = CoreVariantValues_InitializeFileSystems(1);
        if (success)
        {
            const int deviceHandle =
                ((int (__fastcall*)(C_00863ba8*, void*, int))
                    self->vtbl[0xAC / 4])(self, nullptr, configuration);
            success = deviceHandle != 0;
            self->field_0004 = static_cast<uint32_t>(deviceHandle);

            if (success)
            {
                success = static_cast<char>(
                    C_00863ba8_CreateAndRegisterEffect(self, nullptr));
                if (success)
                {
                    success = static_cast<char>(
                        C_00863ba8_InitializeEffectState(self, nullptr));
                    if (success)
                    {
                        uint32_t* dictionary = Audio_CreateEffectDictionary(0);
                        RpMTEffectDictSetCurrent(
                            reinterpret_cast<int>(dictionary), 0);
                        C_00863ba8_FinalizeEffectState(self, nullptr);

                        ((void (__fastcall*)(C_00863ba8*, void*))
                            self->vtbl[0xC8 / 4])(self, nullptr);

                        const uint32_t speakerConfiguration =
                            static_cast<uint32_t>(
                                DSound_MapSpeakerConfiguration(self, nullptr));
                        self->field_0018 = speakerConfiguration;
                        self->field_001c = speakerConfiguration;

                        ((void (__fastcall*)(C_00863ba8*, void*, int))
                            self->vtbl[0x5C / 4])(self, nullptr, 0);
                    }
                }
            }
        }
    }

    const uint32_t deviceHandle = self->field_0004;
    if (deviceHandle)
    {
        const int effectHandle = Audio_FindEffectHandle(
            reinterpret_cast<int*>(deviceHandle), nullptr, 0, nullptr, 0);
        AudioEffect_ReleaseHandle_7DEF60(
            reinterpret_cast<void*>(effectHandle), nullptr, 0);
    }

    return success;
}

HOOK(0x00710AC0, C_00863ba8_InitializeAudio, "auto");
