#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(std::uint32_t, __fastcall, SceneLoader_DestroyMenuBuffer, 0x004FF3C0, (std::uint32_t* self, void*));
CSF_FUNC(int, __fastcall, MenuBuffer_Init, 0x00501870, (uint8_t* self, void*));
CSF_FUNC(int, __fastcall, CMenuMessageBox_SetText, 0x00529030, (void* self, void* , int a2));
CSF_FUNC(int, __fastcall, MenuMessageBox_ApplyConfiguration, 0x0052BB50, (void* self, void* , int configuration));

CSF_GLOBAL(const char*, g_pszMenu_P_MsgBox, 0x00899B28);

static int __fastcall GameBddLoader_ShowSaveGameMessage(
    void* self, void* /*edx*/, int text, int messageAction, int configuration)
{
    if (!CSF_AT(void*, self, 0xF0))
    {
        void* menu = CSF_AT(void*, self, 0x14);
        const char* messageBoxName = g_pszMenu_P_MsgBox;
        CSF_AT(void*, self, 0xF0) =
            ((void* (__fastcall*)(void*, void*, const char*))
                CSF_AT(void**, menu, 0)[0x58 / 4])(
                    menu, nullptr, messageBoxName);
    }

    void* messageBox = CSF_AT(void*, self, 0xF0);
    CSF_AT(int, messageBox, 0x7C) = messageAction;
    CSF_AT(uint8_t, messageBox, 0x80) = 1;

    CMenuMessageBox_SetText(CSF_AT(void*, self, 0xF0), nullptr, text);

    int result;
    if (configuration)
    {
        result = MenuMessageBox_ApplyConfiguration(
            CSF_AT(void*, self, 0xF0), nullptr, configuration);
    }
    else
    {
        alignas(8) uint8_t menuBuffer[0xB4];
        MenuBuffer_Init(menuBuffer, nullptr);

        void* configuredMessageBox = CSF_AT(void*, self, 0xF0);
        CSF_AT(std::uint32_t, menuBuffer, 0x00) = 1;
        CSF_AT(std::uint32_t, menuBuffer, 0x04) = 0;
        MenuMessageBox_ApplyConfiguration(
            configuredMessageBox, nullptr, reinterpret_cast<int>(menuBuffer));
        result = static_cast<int>(SceneLoader_DestroyMenuBuffer(
            reinterpret_cast<std::uint32_t*>(menuBuffer), nullptr));
    }

    CSF_AT(std::uint32_t, self, 0xF4) = 0;
    return result;
}

HOOK(0x0050F860, GameBddLoader_ShowSaveGameMessage, "auto");
