#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CMenuDemoExit_ReleaseGui, 0x00542BA0, (CMenuDemoExit* self, void*));
CSF_FUNC(bool, __fastcall, C_00863ac0_CallSlotAcAndStore, 0x0070F0C0, (C_00863ac0* self, void* , int a2));
CSF_FUNC(int, __fastcall, C_00863ba8_CreateAndRegisterEffect, 0x0070F0F0, (C_00863ba8* self, void*));
CSF_FUNC(int, __fastcall, C_00863ac0_InitializeEffectResources, 0x0070F160, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00863ac0_ApplyEffectResources, 0x0070F1D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00863ac0_ReleaseEffectResources, 0x0070F220, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00863ac0_ReleaseHandleAndClear, 0x0070F280, (C_00863ac0* self, void*));
CSF_FUNC(int, __fastcall, C_00863ac0_ClearHandle, 0x0070F2A0, (C_00863ac0* self, void*));
CSF_FUNC(int, __fastcall, DSound_MapSpeakerConfiguration, 0x007108B0, (C_00863ba8* self, void*));
CSF_FUNC(uint32_t*, __cdecl, Audio_CreateEffectDictionary, 0x007DBBA0, (int a1));
CSF_FUNC(int, __cdecl, Audio_DestroyEffectDictionary, 0x007DBC20, (int a1));
CSF_FUNC(int, __cdecl, RpMTEffectDictSetCurrent, 0x007DBE80, (int a1, int a2));
CSF_FUNC(int, __cdecl, Audio_GetCurrentEffectDictionary, 0x007DBE90, ());

static char __fastcall C_00863ac0_RecreateAudioDevice(
    C_00863ac0* self, void* /*edx*/, int device)
{
    if (!self->field_0038)
        return 1;

    const int speakerConfiguration = DSound_MapSpeakerConfiguration(self, nullptr);

    void* controls =
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x70 / 4])(
            self, nullptr);
    const float firstSetting = static_cast<float>(
        ((double (__fastcall*)(void*, void*))CSF_AT(void**, controls, 0)[0x20 / 4])(
            controls, nullptr));

    controls =
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x70 / 4])(
            self, nullptr);
    const float secondSetting = static_cast<float>(
        ((double (__fastcall*)(void*, void*))CSF_AT(void**, controls, 0)[0x28 / 4])(
            controls, nullptr));

    CMenuDemoExit_ReleaseGui(reinterpret_cast<CMenuDemoExit*>(self), nullptr);
    C_00863ac0_ReleaseEffectResources(self, nullptr);

    if (Audio_GetCurrentEffectDictionary())
        Audio_DestroyEffectDictionary(Audio_GetCurrentEffectDictionary());

    C_00863ac0_ClearHandle(self, nullptr);
    C_00863ac0_ReleaseHandleAndClear(self, nullptr);

    char result = static_cast<char>(
        C_00863ac0_CallSlotAcAndStore(self, nullptr, device));
    if (!result)
        return result;

    result = static_cast<char>(
        C_00863ba8_CreateAndRegisterEffect(self, nullptr));
    if (!result)
        return result;

    result = static_cast<char>(
        C_00863ac0_InitializeEffectResources(self, nullptr));
    if (!result)
        return result;

    uint32_t* dictionary = Audio_CreateEffectDictionary(0);
    RpMTEffectDictSetCurrent(reinterpret_cast<int>(dictionary), 0);
    C_00863ac0_ApplyEffectResources(self, nullptr);

    controls =
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x70 / 4])(
            self, nullptr);
    ((void (__fastcall*)(void*, void*, float))CSF_AT(void**, controls, 0)[0x1C / 4])(
        controls, nullptr, firstSetting);

    controls =
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x70 / 4])(
            self, nullptr);
    ((void (__fastcall*)(void*, void*, float))CSF_AT(void**, controls, 0)[0x24 / 4])(
        controls, nullptr, secondSetting);

    ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[0x68 / 4])(
        self, nullptr, speakerConfiguration);
    ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[0x5C / 4])(
        self, nullptr, 0);

    return result;
}

HOOK(0x0070ED20, C_00863ac0_RecreateAudioDevice, "auto");
