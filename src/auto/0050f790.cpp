#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(std::uint32_t, __fastcall, SceneLoader_DestroyMenuBuffer, 0x004FF3C0, (std::uint32_t* self, void*));
CSF_FUNC(int, __fastcall, MenuBuffer_Init, 0x00501870, (uint8_t* self, void*));
CSF_FUNC(int, __fastcall, CMenuMessageBox_TryAttachResolvedEntry, 0x00529060, (void* self, void* , int a2, int a3));
CSF_FUNC(int, __fastcall, MenuMessageBox_ApplyConfiguration, 0x0052BB50, (void* self, void* , int configuration));

CSF_GLOBAL(const char*, g_pszMenu_P_MsgBox, 0x00899B28);

static int __fastcall GameBddLoader_DispatchMenuRequest(
    void* self, void* /*edx*/, int entry, int entryArgument,
    int requestData, int configuration)
{
    if (!CSF_AT(void*, self, 0xF0))
    {
        void* menu = CSF_AT(void*, self, 0x14);
        CSF_AT(void*, self, 0xF0) =
            ((void*(__fastcall*)(void*, void*, const char*))
                CSF_AT(void**, menu, 0)[0x58 / 4])(
                    menu, nullptr, g_pszMenu_P_MsgBox);
    }

    void* messageBox = CSF_AT(void*, self, 0xF0);
    CSF_AT(int, messageBox, 0x7C) = requestData;
    CSF_AT(uint8_t, messageBox, 0x80) = 1;

    CMenuMessageBox_TryAttachResolvedEntry(
        CSF_AT(void*, self, 0xF0), nullptr, entry, entryArgument);

    int result;
    if (configuration)
    {
        result = MenuMessageBox_ApplyConfiguration(
            CSF_AT(void*, self, 0xF0), nullptr, configuration);
    }
    else
    {
        alignas(8) uint8_t configurationBytes[0xB4];
        MenuBuffer_Init(configurationBytes, nullptr);

        messageBox = CSF_AT(void*, self, 0xF0);
        CSF_AT(std::uint32_t, configurationBytes, 0) = 1;
        CSF_AT(std::uint32_t, configurationBytes, 4) = 0;

        MenuMessageBox_ApplyConfiguration(
            messageBox, nullptr, reinterpret_cast<int>(configurationBytes));
        result = static_cast<int>(SceneLoader_DestroyMenuBuffer(
            reinterpret_cast<std::uint32_t*>(configurationBytes), nullptr));
    }

    CSF_AT(std::uint32_t, self, 0xF4) = 0;
    return result;
}

HOOK(0x0050F790, GameBddLoader_DispatchMenuRequest, "auto");
