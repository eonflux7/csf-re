#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __fastcall, SceneLoader_DestroyMenuBuffer, 0x004FF3C0, (std::uint32_t* self, void*));
CSF_FUNC(int, __fastcall, MenuBuffer_Init, 0x00501870, (uint8_t* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_DispatchMenuRequest, 0x0050F790, (void* self, void* , int entry, int entryArgument, int requestData, int configuration));
CSF_FUNC(int, __fastcall, MenuAccounts_FallbackToList, 0x0055C9D0, (char* self, void* edx));
CSF_FUNC(unsigned int, __cdecl, ConvertWideTextToNarrow, 0x00695620, (std::uint16_t* source, char* destination, unsigned int length));
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(void*, __fastcall, GetContainerDataPtr, 0x00721270, (void* self, void*));

static std::uint32_t __fastcall S_0055c780_SubmitAccountChanges(
    S_0055c780* self, void* /*edx*/)
{
    if (std::strcmp(&CSF_AT(char, self, 0x28C),
                    &CSF_AT(char, self, 0x5D)) == 0)
    {
        self->field_0280 = 1;
        return MenuAccounts_FallbackToList(
            reinterpret_cast<char*>(self), nullptr);
    }

    alignas(8) char otherText[0x14] = {};
    alignas(8) char accountText[0x14] = {};
    void* accountControl = reinterpret_cast<void*>(self->field_002c);
    self->field_0280 = 6;

    if (accountControl)
    {
        void* text = GetContainerDataPtr(accountControl, nullptr);
        ConvertWideTextToNarrow(
            static_cast<std::uint16_t*>(text), accountText, 20);
    }

    void* otherControl = reinterpret_cast<void*>(self->field_0028);
    if (otherControl)
    {
        void* text = GetContainerDataPtr(otherControl, nullptr);
        ConvertWideTextToNarrow(
            static_cast<std::uint16_t*>(text), otherText, 20);
    }

    void* accountService = reinterpret_cast<void*>(GetGlobalState_6A6D70());
    ((void(__fastcall*)(void*, void*, uint8_t*, char*, uint8_t*,
                       int, int, int, int))
        CSF_AT(void**, accountService, 0)[0x30 / 4])(
            accountService, nullptr,
            &CSF_AT(uint8_t, self, 0x3C),
            accountText,
            &CSF_AT(uint8_t, self, 0x5D),
            0, 0, 0, 0);

    alignas(8) uint8_t menuBuffer[0xB4];
    MenuBuffer_Init(menuBuffer, nullptr);
    menuBuffer[24] = 1;

    void* loader = reinterpret_cast<void*>(GetGlobalState_50D190());
    GameBddLoader_DispatchMenuRequest(
        loader, nullptr, 1095053620, 1, 0,
        reinterpret_cast<int>(menuBuffer));

    return SceneLoader_DestroyMenuBuffer(
        reinterpret_cast<std::uint32_t*>(menuBuffer), nullptr);
}

HOOK(0x0055CF60, S_0055c780_SubmitAccountChanges, "auto");
