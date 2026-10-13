#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(char, __stdcall, SceneMissionSetup_ReadAvailableStartCharacters, 0x004E8CE0, (const char* missionPath, bool* hasCommandoStart, bool* hasSniperStart, bool* hasSpyStart));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __fastcall, S_00692b80_GetChildCount, 0x00692B80, (S_00692b80* self, void*));
CSF_FUNC(S_00692b80*, __fastcall, S_00692b80_GetComponent, 0x00692BD0, (S_00692b80* self, void* , unsigned int index));
CSF_FUNC(S_00692b80*, __fastcall, LookupNamedElement, 0x00693360, (S_00692b80* self, void* , char* String1));
CSF_FUNC(unsigned char, __cdecl, GuiBuilder_ReadTwoComponents, 0x0071DE40, (float* destination, unsigned char* node));
CSF_FUNC(unsigned char, __cdecl, GuiBuilder_ReadTwoFloatComponents, 0x0071DEA0, (float* destination, unsigned char* node));
CSF_FUNC(char, __cdecl, ReadFourComponentProperty, 0x0071DF00, (float* components, std::uint8_t* properties));

static const auto kChildCountVtableReference = CSF_VTBL(0x00692B80);

static char __fastcall CMenuLoading_UpdatePortraits(
    CMenuLoading* self, void* /*edx*/, int source)
{
    self->field_0051 = 0;
    S_00692b80* root = CSF_AT(
        S_00692b80*, reinterpret_cast<void*>(source), 0);

    bool photosLoaded = false;
    S_00692b80* photos = LookupNamedElement(
        root, nullptr, const_cast<char*>(CSF_STR(0x008421C4, ".DATOS_FOTOS_PC")));
    if (photos && S_00692b80_GetChildCount(photos, nullptr) == 3)
    {
        float* destination = &CSF_AT(float, self, 0x64);
        for (unsigned int index = 0; index < 3; ++index)
        {
            S_00692b80* component =
                S_00692b80_GetComponent(photos, nullptr, index);
            auto* node = reinterpret_cast<std::uint8_t*>(component);
            GuiBuilder_ReadTwoComponents(destination - 2, node);
            GuiBuilder_ReadTwoFloatComponents(destination, node);
            ReadFourComponentProperty(destination + 2, node);
            destination += 8;
        }
        photosLoaded = true;
    }

    bool clipsLoaded = false;
    S_00692b80* clips = LookupNamedElement(
        root, nullptr, const_cast<char*>(CSF_STR(0x008421B4, ".DATOS_CLIPS_PC")));
    if (clips && S_00692b80_GetChildCount(clips, nullptr) == 3)
    {
        float* destination = &CSF_AT(float, self, 0xC4);
        for (unsigned int index = 0; index < 3; ++index)
        {
            S_00692b80* component =
                S_00692b80_GetComponent(clips, nullptr, index);
            auto* node = reinterpret_cast<std::uint8_t*>(component);
            GuiBuilder_ReadTwoComponents(destination - 2, node);
            GuiBuilder_ReadTwoFloatComponents(destination, node);
            ReadFourComponentProperty(destination + 2, node);
            destination += 8;
        }
        clipsLoaded = true;
    }

    self->field_0051 = photosLoaded && clipsLoaded;
    self->field_0058 = 0;
    CSF_AT(std::uint8_t, self, 0x59) = 0;
    self->field_005a = 0;

    auto* missionState = reinterpret_cast<std::uint8_t*>(GetGlobalValue_8B0568());
    const std::uint32_t capacity = CSF_AT(std::uint32_t, missionState, 0x94);
    const char* missionPath;
    if (capacity < 0x10)
        missionPath = reinterpret_cast<const char*>(missionState + 0x7C + 4);
    else
        missionPath = CSF_AT(const char*, missionState, 0x80);

    GetValue8B0554();
    const char result = SceneMissionSetup_ReadAvailableStartCharacters(
        missionPath,
        reinterpret_cast<bool*>(&self->field_0058),
        reinterpret_cast<bool*>(&CSF_AT(std::uint8_t, self, 0x59)),
        reinterpret_cast<bool*>(&self->field_005a));

    if (!result)
        return static_cast<char>(GetGlobalValue_8B0568());
    return result;
}

HOOK(0x00538DD0, CMenuLoading_UpdatePortraits, "auto");
