#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, SaveGame_DestroyLocalizedString, 0x00428E90, (void* self, void*));
CSF_FUNC(int, __fastcall, MenuScenarioSelect_RebuildSecondarySelection, 0x0052EDE0, (void* self, void* edx, int a3));
CSF_FUNC(std::uint8_t, __fastcall, MenuScenarioSelect_GetPrimarySelectionCode, 0x0052EF80, (void* self, void* , std::uint32_t* selectionCode));
CSF_FUNC(std::uint8_t, __fastcall, MenuScenarioSelect_GetSecondarySelectionCode, 0x0052EFE0, (void* self, void* , std::uint32_t* selectionCode));
CSF_FUNC(char, __fastcall, MenuScenarioSelect_TryQueryEntry, 0x0052F060, (void* self, void* , uint32_t* out));
CSF_FUNC(int, __fastcall, MenuScenarioSelect_RefreshDetails, 0x0052F210, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, MenuScenarioSelect_RefreshEntryList, 0x0052F540, (void* self, void* edx));

static int __fastcall MenuScenarioSelect_UpdateSelection(void* self, void* /*edx*/)
{
    if (!CSF_AT(std::uint32_t, self, 0x18))
        return 0;
    if (!CSF_AT(std::uint32_t, self, 0x20))
        return 0;
    if (!CSF_AT(std::uint32_t, self, 0x28))
        return 0;
    if (!CSF_AT(std::uint32_t, self, 0x30))
        return 0;

    alignas(8) std::uint8_t localizedStringBytes[0x1C];
    alignas(8) std::uint8_t selectionBytes[0x04];
    std::uint32_t& selectionCode =
        *reinterpret_cast<std::uint32_t*>(selectionBytes);

    CSF_AT(std::uint32_t, localizedStringBytes, 0x18) = 7;
    CSF_AT(std::uint32_t, localizedStringBytes, 0x14) = 0;
    CSF_AT(std::uint16_t, localizedStringBytes, 0x04) = 0;

    if (!MenuScenarioSelect_GetPrimarySelectionCode(self, nullptr, &selectionCode))
        return SaveGame_DestroyLocalizedString(localizedStringBytes, nullptr);

    const std::uint32_t primaryCode = selectionCode;
    if (primaryCode != CSF_AT(std::uint32_t, self, 0x40))
    {
        MenuScenarioSelect_RebuildSecondarySelection(
            self, nullptr, static_cast<int>(primaryCode));
    }

    if (!MenuScenarioSelect_GetSecondarySelectionCode(self, nullptr, &selectionCode))
        return SaveGame_DestroyLocalizedString(localizedStringBytes, nullptr);

    const std::uint32_t secondaryCode = selectionCode;
    if (primaryCode == CSF_AT(std::uint32_t, self, 0x40) &&
        secondaryCode == CSF_AT(std::uint32_t, self, 0x44))
    {
        if (!MenuScenarioSelect_TryQueryEntry(self, nullptr, &selectionCode))
            return SaveGame_DestroyLocalizedString(localizedStringBytes, nullptr);

        if (selectionCode == CSF_AT(std::uint32_t, self, 0x48))
            return SaveGame_DestroyLocalizedString(localizedStringBytes, nullptr);

        CSF_AT(std::uint32_t, self, 0x48) = selectionCode;
    }
    else
    {
        void* control = CSF_AT(void*, self, 0x24);
        CSF_AT(std::uint32_t, self, 0x40) = primaryCode;
        CSF_AT(std::uint32_t, self, 0x44) = secondaryCode;

        const int controlFlag = primaryCode ? 0 : 1;
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, control, 0)[0xA8 / 4])(
                control, nullptr, controlFlag);

        control = CSF_AT(void*, self, 0x28);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, control, 0)[0xA8 / 4])(
                control, nullptr, controlFlag);

        control = CSF_AT(void*, self, 0x2C);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, control, 0)[0xA8 / 4])(
                control, nullptr, controlFlag);

        control = CSF_AT(void*, self, 0x30);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, control, 0)[0xA8 / 4])(
                control, nullptr, controlFlag);

        if (primaryCode)
        {
            control = CSF_AT(void*, self, 0x30);
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, control, 0)[0x2B8 / 4])(control, nullptr);

            MenuScenarioSelect_RefreshEntryList(self, nullptr);
            MenuScenarioSelect_TryQueryEntry(
                self, nullptr, &CSF_AT(std::uint32_t, self, 0x48));
        }
    }

    MenuScenarioSelect_RefreshDetails(self, nullptr);
    return SaveGame_DestroyLocalizedString(localizedStringBytes, nullptr);
}

HOOK(0x0052F6B0, MenuScenarioSelect_UpdateSelection, "auto");
