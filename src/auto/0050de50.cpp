#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, GameBddLoader_InitializeMenuAuxiliaryState, 0x004FBBA0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, RwFreeListPurgeAllFreeLists, 0x006265F0, ());
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

CSF_GLOBAL(const char*, g_pszMenu_P_MOG, 0x00899ABC);
CSF_GLOBAL(const char*, g_pszMenu_P_Option, 0x00899ACC);
CSF_GLOBAL(const char*, g_pszMenu_P_Credits, 0x00899AD0);
CSF_GLOBAL(const char*, g_pszMenu_P_Campan, 0x00899AD8);
CSF_GLOBAL(const char*, g_pszMenu_P_SShots, 0x00899ADC);
CSF_GLOBAL(const char*, g_pszMenu_P_Profil, 0x00899AE0);
CSF_GLOBAL(const char*, g_pszMenu_P_MePrin, 0x00899AE4);
CSF_GLOBAL(const char*, g_pszMenu_P_Edit, 0x00899B00);

static char __fastcall CMenuManager_InitializeMenuPanels(
    CMenuManager* self, void* /*edx*/)
{
    void* tacticScreen = reinterpret_cast<void*>(self->m_pScreenTactic);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, tacticScreen, 0)[0x3C / 4])(tacticScreen, nullptr);

    RwFreeListPurgeAllFreeLists();

    self->vSlot2C(
        static_cast<int>(self->m_pScreenProfiles),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(g_pszMenu_P_Profil)));
    self->vSlot2C(
        static_cast<int>(self->field_0028),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(g_pszMenu_P_Campan)));
    self->vSlot2C(
        static_cast<int>(self->field_003c),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(g_pszMenu_P_Option)));
    self->vSlot2C(
        static_cast<int>(self->field_0040),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(g_pszMenu_P_Credits)));
    self->vSlot2C(
        static_cast<int>(self->m_pScreenMainMenu),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(g_pszMenu_P_MePrin)));
    self->vSlot2C(
        static_cast<int>(self->field_0034),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(g_pszMenu_P_Edit)));
    self->vSlot2C(
        static_cast<int>(self->m_pScreenGameOptions),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(g_pszMenu_P_MOG)));
    self->vSlot2C(
        static_cast<int>(self->field_002c),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(g_pszMenu_P_SShots)));

    void* auxiliaryObject = reinterpret_cast<void*>(self->field_007c);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, auxiliaryObject, 0)[0x10 / 4])(auxiliaryObject, nullptr);

    auto* state = static_cast<std::uint8_t*>(Mem_Alloc(12));
    if (state)
        state[0] = 0;

    self->field_0078 =
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(state));
    return GameBddLoader_InitializeMenuAuxiliaryState(state, nullptr);
}

HOOK(0x0050DE50, CMenuManager_InitializeMenuPanels, "auto");
