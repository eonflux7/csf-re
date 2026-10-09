#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(uint32_t, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(uint32_t, g_ScreenHeight, 0x00899BA0);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Hud_UpdatePrimaryWidget, 0x00459240, (void* self, void* edx, float a2));
CSF_FUNC(char, __fastcall, Hud_RefreshControls, 0x00430BB0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, Hud_UpdateTransitions, 0x00430D20, (void* self, void* edx, float elapsed));
CSF_FUNC(char, __fastcall, Hud_UpdateAspectLayout, 0x0042E9E0, (void* self, void* edx, float elapsed));
CSF_FUNC(int, __cdecl, GetGlobal8B0558, 0x004F4920, ());
CSF_FUNC(void, __fastcall, Hud_UpdateSharedPanel, 0x004F49F0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, Hud_UpdateIndicators, 0x0042F2D0, (void* self, void* edx, float elapsed));
CSF_FUNC(void, __fastcall, Hud_RefreshOverlay, 0x00433100, (CHud* self, void* edx));
CSF_FUNC(char, __fastcall, Hud_RefreshStatus, 0x0042DB80, (void* self, void* edx));
CSF_FUNC(char, __fastcall, Hud_UpdateSecondaryWidget, 0x0043A3E0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, Hud_UpdateEnabledWidget, 0x0043B4E0, (void* self, void* edx, float a2));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SetSubtitlePanelSize, 0x004F1D70, (void* self, void* edx, float width, float height));
CSF_FUNC(int, __fastcall, Hud_UpdateSubtitlePanel, 0x004F3370, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, Hud_UpdateAuxiliaryWidget, 0x00428300, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, Hud_UpdateNotificationWidget, 0x00438690, (void* self, void* edx, float a2));

static void __fastcall C_00830a04_Update(CHud* self, void* /*edx*/, float elapsed)
{
    if (!GetSceneMenuContext(g_pCurrentScene, nullptr))
        return;

    if (self->field_0248)
        Hud_UpdatePrimaryWidget(reinterpret_cast<void*>(self->field_0248), nullptr, elapsed);

    Hud_RefreshControls(self, nullptr);
    Hud_UpdateTransitions(self, nullptr, elapsed);
    Hud_UpdateAspectLayout(self, nullptr, elapsed);

    int elapsedBits;
    std::memcpy(&elapsedBits, &elapsed, sizeof(elapsedBits));
    void* sharedPanel = reinterpret_cast<void*>(GetGlobal8B0558());
    Hud_UpdateSharedPanel(sharedPanel, nullptr, elapsedBits);

    Hud_UpdateIndicators(self, nullptr, elapsed);
    Hud_RefreshOverlay(self, nullptr);
    Hud_RefreshStatus(self, nullptr);

    if (self->field_024c)
        Hud_UpdateSecondaryWidget(reinterpret_cast<void*>(self->field_024c), nullptr);
    if (self->field_011c)
        Hud_UpdateEnabledWidget(reinterpret_cast<void*>(self->field_00bc), nullptr, elapsed);

    void* object = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* dimensions = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);
    uint32_t width = CSF_AT(uint32_t, dimensions, 0);

    object = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    dimensions = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);
    float normalizedHeight = static_cast<float>(CSF_AT(uint32_t, dimensions, 4))
                           / static_cast<float>(g_ScreenHeight);
    float normalizedWidth = static_cast<float>(width) / static_cast<float>(g_ScreenWidth);

    SetSubtitlePanelSize(reinterpret_cast<void*>(self->m_subtitlePanel),
                         nullptr, normalizedWidth, normalizedHeight);
    Hud_UpdateSubtitlePanel(reinterpret_cast<void*>(self->m_subtitlePanel), nullptr, elapsed);

    if (self->field_02cc)
        Hud_UpdateAuxiliaryWidget(reinterpret_cast<void*>(self->field_02cc), nullptr, elapsed);
    if (self->field_02d0)
        Hud_UpdateNotificationWidget(reinterpret_cast<void*>(self->field_02d0), nullptr, elapsed);

    float phase;
    std::memcpy(&phase, &self->field_012c, sizeof(phase));
    phase = elapsed + phase;
    std::memcpy(&self->field_012c, &phase, sizeof(phase));
    if (phase >= g_TransitionOne) {
        phase -= g_TransitionOne;
        std::memcpy(&self->field_012c, &phase, sizeof(phase));
    }
}

HOOK(0x00433650, C_00830a04_Update, "auto");
