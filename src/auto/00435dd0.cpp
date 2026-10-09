#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, CHud_DrawWidgetEntries, 0x0042EC60, (CHud* self, void* edx));
CSF_FUNC(void, __cdecl, sub_42F610, 0x0042F610, (int* a1));
CSF_FUNC(int, __fastcall, sub_431780, 0x00431780, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_431A30, 0x00431A30, (void* self, void* edx));
CSF_FUNC(void, __fastcall, sub_4337E0, 0x004337E0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(void, __fastcall, sub_433870, 0x00433870, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_4350C0, 0x004350C0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_4383A0, 0x004383A0, (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, CGuiControl_GetField012C, 0x0044F630, (CGuiControl* self, void*));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuExtras_UpdateThumbnailObject, 0x004F2930, (int* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __stdcall, sub_6C7B40, 0x006C7B40, (int a1, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// The packet also lists this callable address as a vtable reference.
[[maybe_unused]] static void** const g_HudModeUpdateVtable =
    CSF_VTBL(0x004337E0);

static void* Hud_GetCurrentControl()
{
    int collection = GetGlobalValue_4FBDE0();
    int firstElement = GetFirstElement(reinterpret_cast<void*>(collection), nullptr);
    return GetCachedObjectProvider(reinterpret_cast<void*>(firstElement), nullptr);
}

static int Hud_GetCurrentMode()
{
    return ResolveEntryLookupValue(Hud_GetCurrentControl(), nullptr);
}

static int __fastcall CHud_Update(CHud* self, void* /*edx*/, void* state)
{
    const bool restrictedMode =
        CSF_AT(uint8_t, state, 0x228) != 0 ||
        Hud_GetCurrentMode() == 4 ||
        Hud_GetCurrentMode() == 3 ||
        Hud_GetCurrentMode() == 9;

    int service = GetGlobalValue_6C5760();
    void* manager = CSF_AT(void*, reinterpret_cast<void*>(service), 0x04);

    // The assembly supplies ECX as well as the two callee-popped arguments.
    reinterpret_cast<int(__fastcall*)(void*, void*, int, int)>(sub_6C7B40)(
        manager, nullptr, 14, 0);

    if (restrictedMode)
    {
        uint32_t controlValue = CGuiControl_GetField012C(
            static_cast<CGuiControl*>(Hud_GetCurrentControl()), nullptr);
        sub_4337E0(self, nullptr, controlValue);

        if (Hud_GetCurrentMode() != 4 &&
            Hud_GetCurrentMode() != 3 &&
            Hud_GetCurrentMode() != 9)
        {
            sub_4350C0(self, nullptr);
        }
    }
    else if (!CSF_AT(uint8_t, state, 0x3A4))
    {
        sub_431780(self, nullptr);
        sub_433870(self, nullptr);
        sub_4350C0(self, nullptr);
        sub_431A30(self, nullptr);
        CHud_DrawWidgetEntries(self, nullptr);

        if (self->field_02d0)
            sub_4383A0(reinterpret_cast<void*>(self->field_02d0), nullptr);
    }

    if (CSF_AT(uint8_t, CSF_AT(void*, g_pCurrentScene, 0xC0), 0x09))
    {
        sub_42F610(reinterpret_cast<int*>(self->m_letterboxBottom));
        sub_42F610(reinterpret_cast<int*>(self->m_letterboxTop));
    }

    return MenuExtras_UpdateThumbnailObject(
        reinterpret_cast<int*>(self->m_subtitlePanel), nullptr);
}

HOOK(0x00435DD0, CHud_Update, "auto");
