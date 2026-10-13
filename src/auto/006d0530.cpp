#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_00862b58_InitializeWithCamera, 0x006F8270, (C_00862b58* self, void* , int effectAddress, int context));
CSF_FUNC(C_00860178*, __cdecl, GfxScreenEffects_CreateEffectObject, 0x006F9140, ());
CSF_FUNC(char, __fastcall, C_00862d48_Initialize_6F9370, 0x006F9370, (C_00862d48* self, void* , int configuration));
CSF_FUNC(C_00860178*, __cdecl, GfxScreenEffects_CreateEffect_6F97C0, 0x006F97C0, ());
CSF_FUNC(bool, __fastcall, C_00862d48_InitializeNoiseEffect, 0x006F99A0, (C_00862d48* self, void* , int initializationArgument));
CSF_FUNC(C_00860178*, __cdecl, GfxScreenEffects_CreateEffect_6F9C30, 0x006F9C30, ());
CSF_FUNC(C_00860178*, __cdecl, GfxScreenEffects_CreateLargeEffect, 0x006F9D40, ());
CSF_FUNC(bool, __fastcall, C_00862c74_Initialize, 0x006FB0B0, (C_00862c74* self, void* , int configuration));
CSF_FUNC(C_00860178*, __cdecl, ScreenEffects_CreateInstance, 0x006FB580, ());
CSF_FUNC(char, __fastcall, C_00862d48_Initialize_6FB670, 0x006FB670, (C_00862d48* self, void* , int effectId));
CSF_FUNC(C_00860178*, __cdecl, GfxScreenEffects_CreateEffect_6FBAE0, 0x006FBAE0, ());
CSF_FUNC(char, __fastcall, C_00862d48_StartEffect, 0x006FBBF0, (C_00862d48* self, void* , int a2));
CSF_FUNC(C_00860178*, __cdecl, GfxScreenEffects_CreateScreenEffect, 0x006FC0B0, ());

static int __fastcall C_00860db0_CreateInitializedScreenEffect(
    C_00860db0* self, void* /*edx*/, uint32_t* configuration)
{
    C_00860178* effect;
    bool initialized;
    const int configurationAddress = reinterpret_cast<int>(configuration);

    switch (*configuration)
    {
    case 0:
        effect = GfxScreenEffects_CreateScreenEffect();
        initialized = C_00862d48_StartEffect(
            reinterpret_cast<C_00862d48*>(effect), nullptr,
            configurationAddress) != 0;
        break;

    case 1:
        effect = GfxScreenEffects_CreateEffect_6FBAE0();
        initialized = C_00862d48_Initialize_6FB670(
            reinterpret_cast<C_00862d48*>(effect), nullptr,
            configurationAddress) != 0;
        break;

    case 2:
        effect = ScreenEffects_CreateInstance();
        initialized = C_00862c74_Initialize(
            reinterpret_cast<C_00862c74*>(effect), nullptr,
            configurationAddress);
        break;

    case 3:
        effect = GfxScreenEffects_CreateLargeEffect();
        if (((bool (__fastcall*)(void*, void*, uint32_t*))
             CSF_AT(void**, effect, 0)[0x38 / 4])(
                effect, nullptr, configuration))
        {
            return reinterpret_cast<int>(effect);
        }
        effect->vSlot4(1);
        return 0;

    case 4:
        effect = GfxScreenEffects_CreateEffect_6F9C30();
        initialized = C_00862d48_InitializeNoiseEffect(
            reinterpret_cast<C_00862d48*>(effect), nullptr,
            configurationAddress);
        break;

    case 5:
        effect = GfxScreenEffects_CreateEffect_6F97C0();
        initialized = C_00862d48_Initialize_6F9370(
            reinterpret_cast<C_00862d48*>(effect), nullptr,
            configurationAddress) != 0;
        break;

    case 6:
        effect = GfxScreenEffects_CreateEffectObject();
        initialized = C_00862b58_InitializeWithCamera(
            reinterpret_cast<C_00862b58*>(effect), nullptr,
            configurationAddress, static_cast<int>(self->field_0024)) != 0;
        break;

    default:
        return 0;
    }

    if (initialized)
        return reinterpret_cast<int>(effect);

    if (effect)
        effect->vSlot4(1);
    return 0;
}

HOOK(0x006D0530, C_00860db0_CreateInitializedScreenEffect, "auto");
