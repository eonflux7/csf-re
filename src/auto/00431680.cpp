#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Hud_GetSelectedHandle, 0x00486F70, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __stdcall, Hud_FindEntryForValue, 0x0042EC20, (int a1, int a2));
CSF_FUNC(int, __fastcall, S_0042eb60_ComputeIndexFromValue, 0x0042EB60, (void* self, void* edx, int a2, int a3));
CSF_FUNC(void, __fastcall, S_0042eb60_UpdateIndicator, 0x00431130, (void* self, void* edx, unsigned int a2, int a3, int a4, char a5));
CSF_FUNC(int, __fastcall, Hud_QueryCurrentCount, 0x00681F40, (void* self, void* edx));
CSF_FUNC(uint64_t, __fastcall, Hud_QueryCapacity, 0x00476A50, (void* self, void* edx));

CSF_FUNC(void, __cdecl, Hud_RenderWidgetAtScaledRect, 0x0042F4F0, (void* widget, const int* rect));

static void __fastcall S_0042eb60_RefreshCurrentSceneHud(
    S_0042eb60* self, void* /*edx*/)
{
    uint32_t context = GetSceneMenuContext(g_pCurrentScene, nullptr);
    if (!context)
        return;

    void* menu = CSF_AT(void*, reinterpret_cast<void*>(context), 0x44);
    if (!menu)
        return;

    void* handle = reinterpret_cast<void*>(Hud_GetSelectedHandle(menu, nullptr));
    if (!handle || !self->field_0122)
        return;

    int* value = reinterpret_cast<int*>(Handle_GetValuePointer(handle, nullptr));
    int* widget = reinterpret_cast<int*>(self->field_00ec);
    if (widget)
        Hud_RenderWidgetAtScaledRect(widget, nullptr);

    if (value)
    {
        int* entry = reinterpret_cast<int*>(Hud_FindEntryForValue(
            *value,
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x108)));
        if (entry)
            Hud_RenderWidgetAtScaledRect(entry, nullptr);
    }

    if (self->field_011b && self->field_0124 && self->field_0125)
    {
        if (((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, handle, 0)[39])(
                handle, nullptr))
        {
            int count = Hud_QueryCurrentCount(handle, nullptr);
            static_cast<void>(CSF_VTBL(0x0042EB60));
            unsigned int index = S_0042eb60_ComputeIndexFromValue(
                self, nullptr, count, 3);

            int currentCount = Hud_QueryCurrentCount(handle, nullptr);
            int capacity = static_cast<int>(Hud_QueryCapacity(handle, nullptr));
            S_0042eb60_UpdateIndicator(
                self, nullptr, index, 3, 1,
                static_cast<char>(currentCount >= capacity));
        }
    }
}

HOOK(0x00431680, S_0042eb60_RefreshCurrentSceneHud, "auto");
