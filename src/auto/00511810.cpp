#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CMenuManager_SetItemText, 0x0050DE30,
         (void* self, void* edx, int a1, int a2));
CSF_FUNC(int, __fastcall, CMenuManager_RefreshBaseText, 0x0050E610,
         (void* self, void* edx));

CSF_GLOBAL(const char*, g_pszMenu_P_Contro, 0x00899AA8);
CSF_GLOBAL(const char*, g_pszMenu_P_ConIng, 0x00899AAC);
CSF_GLOBAL(const char*, g_pszMenu_P_Video, 0x00899AB0);
CSF_GLOBAL(const char*, g_pszMenu_P_VideoI, 0x00899AB4);
CSF_GLOBAL(const char*, g_pszMenu_P_Sound, 0x00899AC4);
CSF_GLOBAL(const char*, g_pszMenu_P_SoundI, 0x00899AC8);
CSF_GLOBAL(const char*, g_pszMenu_P_Scenes, 0x00899AE8);
CSF_GLOBAL(const char*, g_pszMenu_P_LoGame, 0x00899B08);
CSF_GLOBAL(const char*, g_pszMenu_P_LoMenu, 0x00899B0C);
CSF_GLOBAL(const char*, g_pszMenu_P_SaGame, 0x00899B10);
CSF_GLOBAL(const char*, g_pszMenu_P_VideoA, 0x00899B84);
CSF_GLOBAL(const char*, g_pszMenu_P_SalDem, 0x00899B98);

static char __fastcall C_008407a8_RefreshMenuText(
    C_008407a8* self, void* /*edx*/)
{
    CMenuManager_RefreshBaseText(self, nullptr);

    CMenuManager_SetItemText(self, nullptr,
        static_cast<int>(self->field_0110),
        reinterpret_cast<int>(g_pszMenu_P_LoGame));
    CMenuManager_SetItemText(self, nullptr,
        static_cast<int>(self->field_0114),
        reinterpret_cast<int>(g_pszMenu_P_LoMenu));
    CMenuManager_SetItemText(self, nullptr,
        static_cast<int>(self->field_011c),
        reinterpret_cast<int>(g_pszMenu_P_Scenes));
    CMenuManager_SetItemText(self, nullptr,
        static_cast<int>(self->field_0118),
        reinterpret_cast<int>(g_pszMenu_P_SaGame));
    CMenuManager_SetItemText(self, nullptr,
        static_cast<int>(self->field_0120),
        reinterpret_cast<int>(g_pszMenu_P_Sound));
    CMenuManager_SetItemText(self, nullptr,
        static_cast<int>(self->field_0124),
        reinterpret_cast<int>(g_pszMenu_P_SoundI));
    CMenuManager_SetItemText(self, nullptr,
        static_cast<int>(self->field_0128),
        reinterpret_cast<int>(g_pszMenu_P_Video));
    CMenuManager_SetItemText(self, nullptr,
        static_cast<int>(self->field_012c),
        reinterpret_cast<int>(g_pszMenu_P_VideoI));
    CMenuManager_SetItemText(self, nullptr,
        static_cast<int>(self->field_0130),
        reinterpret_cast<int>(g_pszMenu_P_Contro));
    CMenuManager_SetItemText(self, nullptr,
        static_cast<int>(self->field_0134),
        reinterpret_cast<int>(g_pszMenu_P_VideoA));
    CMenuManager_SetItemText(self, nullptr,
        static_cast<int>(self->field_0138),
        reinterpret_cast<int>(g_pszMenu_P_ConIng));

    const char result = static_cast<char>(self->field_0140);
    if (result)
    {
        return static_cast<char>(CMenuManager_SetItemText(self, nullptr,
            static_cast<int>(self->field_0064),
            reinterpret_cast<int>(g_pszMenu_P_SalDem)));
    }
    return result;
}

HOOK(0x00511810, C_008407a8_RefreshMenuText, "auto");
