#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_0054ecf0_ApplyPreviewModes, 0x0054ECF0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, S_0054ecf0_ApplyPreviewConfiguration, 0x0054F280, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(char, __fastcall, S_0054ecf0_UpdateCharacterPreview, 0x00550490, (void* self, void* edx));
CSF_FUNC(char, __fastcall, S_0054ecf0_UpdateSelectionPreview, 0x005505F0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, S_0054ecf0_UpdateCameraPreview, 0x00550870, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int, __cdecl, GetValue_8B08F8, 0x0055A9C0, ());
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, NetSession_SetPrudpConnectionMode, 0x0073C4A0, (void* self, void* , int requestedMode));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);

static int __fastcall S_0054ecf0_RenderCameraPreview(
    S_0054ecf0* self, void* /*edx*/, float elapsedTime)
{
    if (!self->field_0004)
        return 0;

    if (!self->field_109b && GetGlobal8B0688())
    {
        GetGlobal8B0688();

        uint32_t* configuration;
        if (g_OnlineMatchFlag)
        {
            configuration = reinterpret_cast<uint32_t*>(
                GetValue_8B08F8() + 172);
        }
        else
        {
            configuration = reinterpret_cast<uint32_t*>(
                GetValue_8B08F8() + 24);
        }

        S_0054ecf0_ApplyPreviewConfiguration(self, nullptr, configuration);
        self->field_109b = 1;
    }

    if (CSF_AT(uint32_t, reinterpret_cast<void*>(GetGlobalDword_73C020()), 0x98))
    {
        const float accumulatedTime = elapsedTime + self->field_1094;
        self->field_1094 = accumulatedTime;

        if (accumulatedTime > g_TransitionOne)
        {
            alignas(8) std::uint8_t modeBytes[0x14];
            auto* modes = reinterpret_cast<uint32_t*>(modeBytes);

            modes[0] = 0;
            modes[1] = 0;
            self->field_1094 = accumulatedTime - g_TransitionOne;
            modes[2] = 0;
            modes[3] = 0;
            modes[4] = 0;

            for (int mode = 0; mode < 5; ++mode)
            {
                void* session = reinterpret_cast<void*>(GetGlobalDword_73C020());
                modes[mode] = NetSession_SetPrudpConnectionMode(
                    session, nullptr, mode);
            }

            // Retain the packet's address binding; the instruction is a direct call.
            (void)CSF_VTBL(0x0054ECF0);
            S_0054ecf0_ApplyPreviewModes(self, nullptr, modes);
        }

        S_0054ecf0_UpdateCharacterPreview(self, nullptr);
        S_0054ecf0_UpdateSelectionPreview(self, nullptr);
        S_0054ecf0_UpdateCameraPreview(self, nullptr);
    }

    const uint32_t previewMember = self->field_0004;
    const uint32_t previewBase = previewMember ? previewMember - 216 : 0;

    const int result =
        reinterpret_cast<int (__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, reinterpret_cast<void*>(previewBase), 0xD8)[0xC / 4])(
                reinterpret_cast<void*>(previewBase + 216), nullptr, elapsedTime);

    self->field_109c = elapsedTime + self->field_109c;
    return result;
}

HOOK(0x00550980, S_0054ecf0_RenderCameraPreview, "auto");
