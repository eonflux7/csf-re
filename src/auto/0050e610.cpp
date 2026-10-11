#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __fastcall, AppendNamedEntry, 0x0071D300, (void* self, void* , int value, char* name, char* alternateText));

CSF_GLOBAL(const char*, g_pszMenu_P_Help,    0x00899AA4);
CSF_GLOBAL(const char*, g_pszMenu_P_MOGIng,  0x00899AB8);
CSF_GLOBAL(const char*, g_pszMenu_P_MOG,     0x00899ABC);
CSF_GLOBAL(const char*, g_pszMenu_P_Idioma,  0x00899AC0);
CSF_GLOBAL(const char*, g_pszMenu_P_Option,  0x00899ACC);
CSF_GLOBAL(const char*, g_pszMenu_P_Credits, 0x00899AD0);
CSF_GLOBAL(const char*, g_pszMenu_P_OptIng,  0x00899AD4);
CSF_GLOBAL(const char*, g_pszMenu_P_Campan,  0x00899AD8);
CSF_GLOBAL(const char*, g_pszMenu_P_SShots,  0x00899ADC);
CSF_GLOBAL(const char*, g_pszMenu_P_Profil,  0x00899AE0);
CSF_GLOBAL(const char*, g_pszMenu_P_MePrin,  0x00899AE4);
CSF_GLOBAL(const char*, g_pszMenu_P_Carga,   0x00899AEC);
CSF_GLOBAL(const char*, g_pszMenu_P_Juego,   0x00899AF0);
CSF_GLOBAL(const char*, g_pszMenu_P_MeJueg,  0x00899AF4);
CSF_GLOBAL(const char*, g_pszMenu_P_Edit,    0x00899B00);
CSF_GLOBAL(const char*, g_pszMenu_P_EditG,   0x00899B04);
CSF_GLOBAL(const char*, g_pszMenu_P_Tactic,  0x00899B14);
CSF_GLOBAL(const char*, g_pszMenu_P_ProgBr,  0x00899B24);
CSF_GLOBAL(const char*, g_pszMenu_P_MsgBox,  0x00899B28);
CSF_GLOBAL(const char*, g_pszMenu_P_Intro,   0x00899B2C);

static hook::Tail __fastcall CMenuManager_RegisterScreens(
    void* self, const uint32_t* /*args*/)
{
    auto* manager = static_cast<CMenuManager*>(self);

    AppendNamedEntry(reinterpret_cast<void*>(manager->field_0014), nullptr,
        static_cast<int>(manager->field_0060),
        const_cast<char*>(g_pszMenu_P_Help), nullptr);
    AppendNamedEntry(reinterpret_cast<void*>(manager->field_0014), nullptr,
        static_cast<int>(manager->field_00ec),
        const_cast<char*>(g_pszMenu_P_Idioma), nullptr);
    AppendNamedEntry(reinterpret_cast<void*>(manager->field_0014), nullptr,
        static_cast<int>(manager->field_0050),
        const_cast<char*>(g_pszMenu_P_EditG), nullptr);
    AppendNamedEntry(reinterpret_cast<void*>(manager->field_0014), nullptr,
        static_cast<int>(manager->m_pScreenGame),
        const_cast<char*>(g_pszMenu_P_Juego), nullptr);
    AppendNamedEntry(reinterpret_cast<void*>(manager->field_0014), nullptr,
        static_cast<int>(manager->m_pScreenGameMenu),
        const_cast<char*>(g_pszMenu_P_MeJueg), nullptr);
    AppendNamedEntry(reinterpret_cast<void*>(manager->field_0014), nullptr,
        static_cast<int>(manager->m_pScreenProfiles),
        const_cast<char*>(g_pszMenu_P_Profil), nullptr);
    AppendNamedEntry(reinterpret_cast<void*>(manager->field_0014), nullptr,
        static_cast<int>(manager->field_0028),
        const_cast<char*>(g_pszMenu_P_Campan), nullptr);
    AppendNamedEntry(reinterpret_cast<void*>(manager->field_0014), nullptr,
        static_cast<int>(manager->field_002c),
        const_cast<char*>(g_pszMenu_P_SShots), nullptr);
    AppendNamedEntry(reinterpret_cast<void*>(manager->field_0014), nullptr,
        static_cast<int>(manager->field_003c),
        const_cast<char*>(g_pszMenu_P_Option), nullptr);
    AppendNamedEntry(reinterpret_cast<void*>(manager->field_0014), nullptr,
        static_cast<int>(manager->field_0040),
        const_cast<char*>(g_pszMenu_P_Credits), nullptr);
    AppendNamedEntry(reinterpret_cast<void*>(manager->field_0014), nullptr,
        static_cast<int>(manager->field_0044),
        const_cast<char*>(g_pszMenu_P_OptIng), nullptr);
    AppendNamedEntry(reinterpret_cast<void*>(manager->field_0014), nullptr,
        static_cast<int>(manager->m_pScreenGameOptionsInGame),
        const_cast<char*>(g_pszMenu_P_MOGIng), nullptr);
    AppendNamedEntry(reinterpret_cast<void*>(manager->field_0014), nullptr,
        static_cast<int>(manager->m_pScreenGameOptions),
        const_cast<char*>(g_pszMenu_P_MOG), nullptr);
    AppendNamedEntry(reinterpret_cast<void*>(manager->field_0014), nullptr,
        static_cast<int>(manager->m_pScreenMainMenu),
        const_cast<char*>(g_pszMenu_P_MePrin), nullptr);
    AppendNamedEntry(reinterpret_cast<void*>(manager->field_0014), nullptr,
        static_cast<int>(manager->m_pScreenLoading),
        const_cast<char*>(g_pszMenu_P_Carga), nullptr);
    AppendNamedEntry(reinterpret_cast<void*>(manager->field_0014), nullptr,
        static_cast<int>(manager->field_0034),
        const_cast<char*>(g_pszMenu_P_Edit), nullptr);
    AppendNamedEntry(reinterpret_cast<void*>(manager->field_0014), nullptr,
        static_cast<int>(manager->m_pScreenTactic),
        const_cast<char*>(g_pszMenu_P_Tactic), nullptr);
    AppendNamedEntry(reinterpret_cast<void*>(manager->field_0014), nullptr,
        static_cast<int>(manager->field_0058),
        const_cast<char*>(g_pszMenu_P_ProgBr), nullptr);
    AppendNamedEntry(reinterpret_cast<void*>(manager->field_0014), nullptr,
        static_cast<int>(manager->m_pScreenMsgBox),
        const_cast<char*>(g_pszMenu_P_MsgBox), nullptr);
    AppendNamedEntry(reinterpret_cast<void*>(manager->field_0014), nullptr,
        static_cast<int>(manager->m_pScreenIntro),
        const_cast<char*>(g_pszMenu_P_Intro), nullptr);

    void* obj = reinterpret_cast<void*>(manager->field_007c);
    return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x08 / 4]);
}

HOOK_TAIL(0x0050E610, CMenuManager_RegisterScreens, "auto");
