#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char, __fastcall, S_004503E0_ForwardWithScratch, 0x00452210, (S_004503e0* self, void* , C_008342e8* a2, float a3));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_004950e0_GetField398, 0x004950E0, (void* self, void*));
CSF_FUNC(int, __fastcall, HudAlertIcons_AddAlertIcon, 0x0049FA50, (void* self, void* edx, void* a2, void* a3, int a4));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static void __fastcall S_004a00e0_UpdateAlertIcon(
    S_004a00e0* self, void* /*edx*/, void* source)
{
    alignas(8) uint8_t handleBytes[0x10];
    CSF_AT(S_004a00e0*, handleBytes, 0x0C) = self;

    if (!source)
        return;

    const int alertState = C_004950e0_GetField398(source, nullptr);
    if (!alertState)
        return;

    CSF_AT(uint32_t, handleBytes, 0x04) = static_cast<uint32_t>(-1);
    CSF_AT(int, handleBytes, 0x08) = -1;

    int handleIndex = CSF_AT(int32_t, source, 0x248);
    CSF_AT(void**, handleBytes, 0x00) = CSF_VTBL(0x0082D448);
    if (handleIndex < 0)
    {
        handleIndex = CSF_AT(int32_t, source, 0x254);
        if (handleIndex < 0)
            return;
        CSF_AT(uint32_t, handleBytes, 0x04) =
            CSF_AT(uint32_t, source, 0x250);
    }
    else
    {
        CSF_AT(uint32_t, handleBytes, 0x04) =
            CSF_AT(uint32_t, source, 0x244);
    }

    CSF_AT(int, handleBytes, 0x08) = handleIndex;
    if (handleIndex < 0)
        return;

    auto* target = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr, reinterpret_cast<int>(handleBytes)));
    if (!target)
        return;

    if ((CSF_AT(uint32_t, target, 0x170) & 0x2000) == 0)
        return;
    if (CSF_AT(int, target, 0x78) != 1)
        return;

    const int globalValue = GetGlobalValue_4FBDE0();
    const int firstElement = GetFirstElement(
        reinterpret_cast<void*>(globalValue), nullptr);
    void* provider = GetCachedObjectProvider(
        reinterpret_cast<void*>(firstElement), nullptr);
    if (!provider)
        return;

    if (ResolveEntryLookupValue(provider, nullptr))
        return;

    if (S_004503E0_ForwardWithScratch(
            reinterpret_cast<S_004503e0*>(g_pCurrentScene),
            nullptr, target, 6000.0f))
    {
        HudAlertIcons_AddAlertIcon(
            CSF_AT(S_004a00e0*, handleBytes, 0x0C),
            nullptr, provider, target, alertState);
    }
}

HOOK(0x004A0330, S_004a00e0_UpdateAlertIcon, "auto");
