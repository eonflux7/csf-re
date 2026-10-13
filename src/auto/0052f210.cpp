#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(C_00833908*, __fastcall, S_004e9090_ResolveObject, 0x004EEEF0, (S_004e9090* self, void* , int nameKey, uint32_t param, char altMode));
CSF_FUNC(int, __fastcall, MpCharacterSelect_SetPreviewCharacter, 0x005422D0, (void* self, void* , void* previewCharacter));
CSF_FUNC(int, __cdecl, GetGlobalValue_558150, 0x00558150, ());

static int __fastcall MenuScenarioSelect_RebuildPreviewCharacter(void* self, void* /*edx*/)
{
    void* previousCharacter = CSF_AT(void*, self, 0x54);
    if (previousCharacter)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, previousCharacter, 0)[0])(
                previousCharacter, nullptr, 1);
        CSF_AT(void*, self, 0x54) = nullptr;
    }

    const int faction = CSF_AT(int, self, 0x40);
    if (!faction)
    {
        return MpCharacterSelect_SetPreviewCharacter(
            CSF_AT(void*, self, 0x58), nullptr, nullptr);
    }

    int characterIndex = 0;
    switch (CSF_AT(int, self, 0x44))
    {
    case 1:
        characterIndex = 0;
        break;
    case 2:
        characterIndex = 1;
        break;
    case 3:
        characterIndex = 2;
        break;
    default:
        // The original uses an uninitialized stack value here.
        characterIndex = 0;
        break;
    }

    int factionIndex = 0;
    if (faction == 2)
        factionIndex = 1;

    void* characterKeys = reinterpret_cast<void*>(GetGlobalValue_558150());
    const int keyIndex =
        factionIndex + 4 + characterIndex + 2 * (factionIndex + 4);
    const int nameKey = CSF_AT(int, characterKeys, 4 * keyIndex);

    auto* objectManager = reinterpret_cast<S_004e9090*>(GetValue8B0554());
    C_00833908* character = S_004e9090_ResolveObject(
        objectManager, nullptr, nameKey, 0xFFFFFFFF, 1);
    CSF_AT(C_00833908*, self, 0x54) = character;

    if (!character)
    {
        return MpCharacterSelect_SetPreviewCharacter(
            CSF_AT(void*, self, 0x58), nullptr, nullptr);
    }

    void* component = reinterpret_cast<void*>(character->field_0044);
    const int count = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, component, 0)[0xA0 / 4])(component, nullptr);

    for (int index = static_cast<int>(static_cast<uint32_t>(count) - 1);
         index >= 0; --index)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, component, 0)[0xB4 / 4])(
                component, nullptr, index);
    }

    void* manager = reinterpret_cast<void*>(GetValue8B0554());
    void* selectionRegistry = CSF_AT(void*, manager, 0x1FC0);
    const int selection = CSF_AT(int, self, 0x48);
    const int selectedObject =
        reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, selectionRegistry, 0)[0x0C / 4])(
                selectionRegistry, nullptr, selection);

    reinterpret_cast<void (__fastcall*)(void*, void*, int, int)>(
        CSF_AT(void**, component, 0)[0xA4 / 4])(
            component, nullptr, selectedObject, 1);

    void* previewCharacter = CSF_AT(void*, self, 0x54);
    void* previewControl = CSF_AT(void*, self, 0x58);
    return MpCharacterSelect_SetPreviewCharacter(
        previewControl, nullptr, previewCharacter);
}

HOOK(0x0052F210, MenuScenarioSelect_RebuildPreviewCharacter, "auto");
