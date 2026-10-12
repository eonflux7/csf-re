#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __cdecl, CommandoAnims_PrepareToggleTransition, 0x004FC000, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, CSoundManager_SetMasterAndEffectsVolumes, 0x005228C0, (CSoundManager* self, void* , float masterVolume, float effectsVolume));
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(char, __fastcall, SoundManager_PlaySoundInSceneMenuContext, 0x00525AB0, (int* self, void* , int soundArg));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void __fastcall C_0083f448_SetToggleMode(
    C_0083f448* self, void* /*edx*/, int mode)
{
    if (!self->field_005c)
        return;

    if (mode != 0)
    {
        switch (self->field_007c)
        {
        case 1:
        case 3:
            return;
        case 5:
            if (mode != 3)
                return;
            break;
        }
    }

    if (self->field_007c == 2)
    {
        auto* soundManager =
            reinterpret_cast<CSoundManager*>(GetGlobalValue_521BB0());
        CSoundManager_SetMasterAndEffectsVolumes(
            soundManager, nullptr, 1.0f, 1.0f);

        auto* menuSoundContext = reinterpret_cast<int*>(Get_008b0678());
        SoundManager_PlaySoundInSceneMenuContext(
            menuSoundContext, nullptr, 61);
    }

    int color = 0xFFFFFF;
    self->field_007c = mode;
    self->field_0080 = 0;
    self->field_0084 = 0;
    self->field_0088 = 0;

    switch (mode)
    {
    case 0:
    case 1:
    case 4:
    case 5:
        break;

    case 2:
    {
        void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        self->field_0080 = CSF_AT(uint32_t, settings, 0x18B0);
        CommandoAnims_PrepareToggleTransition();
        break;
    }

    case 3:
    {
        void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        color = CSF_AT(uint32_t, settings, 0x18AC) & 0xFFFFFF;
        break;
    }

    default:
        self->field_007c = 0;
        break;
    }

    const bool enabled = self->field_007c != 0;
    void* entry = nullptr;
    if (self->field_001c)
        entry = CSF_AT(void*, reinterpret_cast<void*>(self->field_0014), 0);

    void* owner =
        reinterpret_cast<void*>(ResolveEntryLookupValue(entry, nullptr));

    if (!enabled)
    {
        ((void(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, owner, 0)[0x3C / 4])(
                owner, nullptr, reinterpret_cast<void*>(self->field_005c));
        return;
    }

    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, owner, 0)[0x30 / 4])(
            owner, nullptr, reinterpret_cast<void*>(self->field_005c));

    void* effect = reinterpret_cast<void*>(self->field_005c);
    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, effect, 0)[0x2C / 4])(effect, nullptr);

    effect = reinterpret_cast<void*>(self->field_005c);
    ((void(__fastcall*)(void*, void*, int, int))
        CSF_AT(void**, effect, 0)[0x1C / 4])(effect, nullptr, 3, color);

    float duration;
    std::memcpy(&duration, &self->field_0080, sizeof(duration));
    if (duration > g_fZero)
    {
        effect = reinterpret_cast<void*>(self->field_005c);
        const int extent = ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, effect, 0)[0x20 / 4])(effect, nullptr, 0);

        const float initialExtent = static_cast<float>(extent);
        std::memcpy(&self->field_0084, &initialExtent, sizeof(initialExtent));

        std::memcpy(&duration, &self->field_0080, sizeof(duration));
        const float rate = static_cast<float>(extent) / duration;
        std::memcpy(&self->field_0088, &rate, sizeof(rate));
    }
}

HOOK(0x004FC210, C_0083f448_SetToggleMode, "auto");
