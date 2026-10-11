#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, CMenuManager_PrepareMenuObjects, 0x0050DE50,
         (void* self, void* edx));

CSF_GLOBAL(const char*, g_pszMenu_P_LoMenu, 0x00899B0C);
CSF_GLOBAL(const char*, g_pszMenu_P_Scenes, 0x00899AE8);
CSF_GLOBAL(const char*, g_pszMenu_P_Sound, 0x00899AC4);
CSF_GLOBAL(const char*, g_pszMenu_P_Video, 0x00899AB0);
CSF_GLOBAL(const char*, g_pszMenu_P_Contro, 0x00899AA8);
CSF_GLOBAL(const char*, g_pszMenu_P_VideoA, 0x00899B84);
CSF_GLOBAL(const char*, g_pszMenu_P_SalDem, 0x00899B98);

static char __fastcall C_008407a8_ConnectMenuObjects(
    C_008407a8* self, void* /*edx*/)
{
    CMenuManager_PrepareMenuObjects(self, nullptr);

    self->vSlot2C(static_cast<int>(self->field_0114),
                  reinterpret_cast<int>(g_pszMenu_P_LoMenu));
    self->vSlot2C(static_cast<int>(self->field_011c),
                  reinterpret_cast<int>(g_pszMenu_P_Scenes));
    self->vSlot2C(static_cast<int>(self->field_0120),
                  reinterpret_cast<int>(g_pszMenu_P_Sound));
    self->vSlot2C(static_cast<int>(self->field_0128),
                  reinterpret_cast<int>(g_pszMenu_P_Video));
    self->vSlot2C(static_cast<int>(self->field_0130),
                  reinterpret_cast<int>(g_pszMenu_P_Contro));
    self->vSlot2C(static_cast<int>(self->field_0134),
                  reinterpret_cast<int>(g_pszMenu_P_VideoA));

    if (self->field_0140)
    {
        self->vSlot2C(static_cast<int>(self->field_0064),
                      reinterpret_cast<int>(g_pszMenu_P_SalDem));
        // The original leaves incidental AL contents from this void call.
    }
    return 0;
}

HOOK(0x00511A40, C_008407a8_ConnectMenuObjects, "auto");
