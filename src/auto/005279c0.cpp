#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(void*, __fastcall, C_0085e7b8_GetDefaultSoundName, 0x0051BA50,
         (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());
CSF_FUNC(int, __fastcall, C_0085e7b8_GetSoundDefinition, 0x0075A480,
         (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_Const200_0f, 0x0082DDEC);
CSF_GLOBAL(float, g_FloatMax, 0x00831090);

static char __fastcall C_008411a0_InitializePlaybackState(
    C_008411a0* self, void* /*edx*/)
{
    self->field_0060 = 0;
    self->field_0048 = 0.0f;
    self->field_004c = 0.0f;
    self->field_0050 = 0.0f;
    self->field_0040 = 0;
    self->field_0044 = 0.0f;
    self->field_0054 = 0;
    self->field_003c = 0;
    self->field_0058 = 1065353216;
    self->field_005c = 1065353216;

    auto* context = reinterpret_cast<void*>(GetValue8B0554());
    auto* definitions = CSF_AT(C_0085e7b8*, context, 0x1FD0);
    const int definitionIndex =
        reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t)>(
            definitions->vtbl[0xC / 4])(
                definitions, nullptr, self->field_000c);

    if (definitionIndex != -1)
    {
        auto* definition = reinterpret_cast<void*>(
            C_0085e7b8_GetSoundDefinition(
                definitions, nullptr, definitionIndex));

        self->field_0044 =
            CSF_AT(float, definition, 0x18) + g_Const200_0f;
        self->field_0054 = CSF_AT(uint8_t, definition, 0x20);

        uint32_t variantCount = CSF_AT(uint32_t, definition, 0x8);
        if (variantCount != 0)
        {
            const int32_t byteCount = static_cast<int32_t>(
                CSF_AT(uint32_t, definition, 0xC) - variantCount);
            variantCount = static_cast<uint32_t>(byteCount / 28);
        }

        char* soundName;
        const uint32_t variantIndex = self->field_0010;
        if (variantIndex >= variantCount)
        {
            void* name = C_0085e7b8_GetDefaultSoundName(
                definitions, nullptr, static_cast<unsigned int>(definitionIndex));
            if (CSF_AT(uint32_t, name, 0x18) < 0x10u)
                soundName = reinterpret_cast<char*>(name) + 4;
            else
                soundName = CSF_AT(char*, name, 0x4);
        }
        else
        {
            const uint32_t variantAddress =
                CSF_AT(uint32_t, definition, 0x8) + 28 * variantIndex;
            auto* variant = reinterpret_cast<void*>(variantAddress);
            if (CSF_AT(uint32_t, variant, 0x18) < 0x10u)
                soundName = reinterpret_cast<char*>(variant) + 4;
            else
                soundName = CSF_AT(char*, variant, 0x4);
        }

        auto* audio = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
        double duration =
            reinterpret_cast<double(__fastcall*)(void*, void*, char*)>(
                CSF_AT(void**, audio, 0)[0x88 / 4])(
                    audio, nullptr, soundName);
        self->field_004c = static_cast<float>(duration);

        if (self->field_0014 > g_fZero)
        {
            self->field_0050 = self->field_0014;
            self->field_003c = 1;
            return self->field_003c;
        }

        if (self->field_0054)
            duration = g_FloatMax;
        self->field_0050 = static_cast<float>(duration);
        self->field_003c = 1;
    }

    return self->field_003c;
}

HOOK(0x005279C0, C_008411a0_InitializePlaybackState, "auto");
