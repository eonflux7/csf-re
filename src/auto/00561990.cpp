#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CMenuMpSessions_CacheListboxPartidas, 0x005612D0, (CMenuMpSessions* self, void* , int , int));
CSF_FUNC(int, __fastcall, CMenuMpSessions_BindAlternateControls, 0x005612F0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, MenuMpSessions_BindListboxes, 0x005615E0, (void* self, void* , int a2, int a3));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360, (uint8_t* self, void* edx, const char* name));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(unsigned int, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(unsigned int, g_ScreenHeight, 0x00899BA0);

static int __fastcall CMenuMpSessions_BindMainPanel(
    CMenuMpSessions* self, void* /*edx*/, void* source, uint8_t** elements)
{
    int result = static_cast<int>(self->field_0040);
    if (result)
        return result;

    void* accessor =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, source, 0)[0x54 / 4])(source, nullptr);
    void** accessorVtable = CSF_AT(void**, accessor, 0);

    uint8_t* panelElement = LookupNamedElement(
        *elements, nullptr, CSF_STR(0x00841304, ".PANEL_PRINCIPAL"));

    result =
        ((int (__fastcall*)(void*, void*, uint8_t*))
            accessorVtable[0x18 / 4])(accessor, nullptr, panelElement);
    self->field_0040 = static_cast<uint32_t>(result);
    if (!result)
        return result;

    void* display = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* dimensions =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);
    uint32_t width = CSF_AT(uint32_t, dimensions, 0);

    display = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    dimensions =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);
    uint32_t height = CSF_AT(uint32_t, dimensions, 0x4);

    void* panel = reinterpret_cast<void*>(self->field_0040);
    void** panelVtable = CSF_AT(void**, panel, 0);
    float normalizedHeight =
        static_cast<float>(height) / static_cast<float>(g_ScreenHeight);
    float normalizedWidth =
        static_cast<float>(width) / static_cast<float>(g_ScreenWidth);

    ((void (__fastcall*)(void*, void*, float, float))
        panelVtable[0x150 / 4])(
            panel, nullptr, normalizedWidth, normalizedHeight);

    result = static_cast<int>(self->field_01e0);
    switch (result)
    {
    case 0:
        return CMenuMpSessions_CacheListboxPartidas(
            self, nullptr, reinterpret_cast<int>(source),
            reinterpret_cast<int>(elements));
    case 1:
    case 3:
        return CMenuMpSessions_BindAlternateControls(
            self, nullptr, reinterpret_cast<int>(source),
            reinterpret_cast<int>(elements));
    case 2:
    case 4:
        return MenuMpSessions_BindListboxes(
            self, nullptr, reinterpret_cast<int>(source),
            reinterpret_cast<int>(elements));
    default:
        return result;
    }
}

HOOK(0x00561990, CMenuMpSessions_BindMainPanel, "auto");
