#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(const char*, g_pszMenu_P_Help,   0x00899AA4);
CSF_GLOBAL(const char*, g_pszMenu_P_MOGIng, 0x00899AB8);
CSF_GLOBAL(const char*, g_pszMenu_P_OptIng, 0x00899AD4);
CSF_GLOBAL(const char*, g_pszMenu_P_Juego,  0x00899AF0);
CSF_GLOBAL(const char*, g_pszMenu_P_EditG,  0x00899B04);
CSF_GLOBAL(const char*, g_pszMenu_P_ProgBr, 0x00899B24);
CSF_GLOBAL(const char*, g_pszMenu_P_MsgBox, 0x00899B28);
CSF_GLOBAL(const char*, g_pszMenu_P_Intro,  0x00899B2C);

static hook::Tail __fastcall CMenuManager_BindInGamePanels(
    void* object, const uint32_t* /*args*/)
{
    auto* self = static_cast<CMenuManager*>(object);
    const char* messageBoxName = g_pszMenu_P_MsgBox;
    void* messageBox = reinterpret_cast<void*>(self->m_pScreenMsgBox);

    if (messageBox)
    {
        int isBound =
            (reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, messageBox, 0)[0x20 / 4]))(
                    messageBox, nullptr);

        if (!isBound)
        {
            void* panelOwner = reinterpret_cast<void*>(self->field_0014);
            int panel =
                (reinterpret_cast<int(__fastcall*)(void*, void*, const char*)>(
                    CSF_AT(void**, panelOwner, 0)[0x88 / 4]))(
                        panelOwner, nullptr, messageBoxName);

            if (panel)
            {
                (reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t, int)>(
                    CSF_AT(void**, messageBox, 0)[0x38 / 4]))(
                        messageBox, nullptr, self->field_0014, panel);

                panelOwner = reinterpret_cast<void*>(self->field_0014);
                (reinterpret_cast<void(__fastcall*)(void*, void*, const char*)>(
                    CSF_AT(void**, panelOwner, 0)[0x8C / 4]))(
                        panelOwner, nullptr, messageBoxName);
            }
        }
    }

    self->vSlot2C(static_cast<int>(self->m_pScreenGame),
                  reinterpret_cast<int>(g_pszMenu_P_Juego));
    self->vSlot2C(static_cast<int>(self->field_0044),
                  reinterpret_cast<int>(g_pszMenu_P_OptIng));
    self->vSlot2C(static_cast<int>(self->m_pScreenGameOptionsInGame),
                  reinterpret_cast<int>(g_pszMenu_P_MOGIng));
    self->vSlot2C(static_cast<int>(self->field_0050),
                  reinterpret_cast<int>(g_pszMenu_P_EditG));
    self->vSlot2C(static_cast<int>(self->field_0058),
                  reinterpret_cast<int>(g_pszMenu_P_ProgBr));
    self->vSlot2C(static_cast<int>(self->m_pScreenIntro),
                  reinterpret_cast<int>(g_pszMenu_P_Intro));
    self->vSlot2C(static_cast<int>(self->field_0060),
                  reinterpret_cast<int>(g_pszMenu_P_Help));

    void* target = reinterpret_cast<void*>(self->field_007c);
    return hook::tail_jump(
        target, (*reinterpret_cast<void***>(target))[0x14 / 4]);
}

HOOK_TAIL(0x0050DF20, CMenuManager_BindInGamePanels, "auto");
