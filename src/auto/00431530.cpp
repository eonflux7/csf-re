#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));
CSF_FUNC(uint64_t, __fastcall, GetItemCapacity, 0x00476A50, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_0042eb60_ResolveSlotIndex, 0x0042EB60, (void* self, void* edx, int a2, int a3));
CSF_FUNC(void, __fastcall, S_0042eb60_UpdateSlotIndicator, 0x00431130, (void* self, void* edx, unsigned int a2, int a3, int a4, char a5));

CSF_FUNC(void, __cdecl, Hud_RenderWidgetAtScaledRect, 0x0042F4F0, (void* widget, const int* rect));

static void __fastcall S_0042eb60_RefreshHudSlots(
    S_0042eb60* self, void* /*edx*/)
{
    if (!self->field_0123)
        return;

    uint32_t context = GetSceneMenuContext(g_pCurrentScene, nullptr);
    if (!context)
        return;

    uint32_t hudRoot = CSF_AT(uint32_t, reinterpret_cast<void*>(context), 0x44);
    if (!hudRoot)
        return;

    void* widgetHost = CSF_AT(void*, reinterpret_cast<void*>(hudRoot), 0x88);
    if (!widgetHost)
        return;

    int firstItemCount = GetCurrentItemCount(widgetHost, nullptr);
    if (!firstItemCount)
        return;

    if (self->field_0123)
    {
        uint32_t widgetCount = self->field_008c;
        uint32_t index = 0;
        if (widgetCount)
        {
            do
            {
                int* widget = reinterpret_cast<int**>(self->field_0084)[index];
                Hud_RenderWidgetAtScaledRect(widget, nullptr);
                ++index;
            } while (index < widgetCount);
        }
    }

    if (self->field_011b && self->field_0124 && self->field_0125)
    {
        static_cast<void>(CSF_VTBL(0x0042EB60));

        unsigned int slot = static_cast<unsigned int>(
            S_0042eb60_ResolveSlotIndex(self, nullptr, firstItemCount, 6));
        int currentItemCount = GetCurrentItemCount(widgetHost, nullptr);
        int capacity = static_cast<int>(GetItemCapacity(widgetHost, nullptr));
        S_0042eb60_UpdateSlotIndicator(
            self, nullptr, slot, 6, 1,
            static_cast<char>(currentItemCount >= capacity));
    }
}

HOOK(0x00431530, S_0042eb60_RefreshHudSlots, "auto");
