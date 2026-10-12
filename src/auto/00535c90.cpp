#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, MenuMPCharSel_InitEntry, 0x00542700, (void* self, void* , int id));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360, (uint8_t* self, void* edx, const char* name));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(unsigned int, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(unsigned int, g_ScreenHeight, 0x00899BA0);

static int __fastcall Interface_InitializeMainPanel(
    void* self, void* /*edx*/, void* scene, uint8_t** elements)
{
    const int existingPanel = CSF_AT(int, self, 0x13C);
    if (existingPanel)
        return existingPanel;

    void* factory =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, scene, 0)[0x54 / 4])(scene, nullptr);
    void** factoryVtable = CSF_AT(void**, factory, 0);

    uint8_t* panelElement = LookupNamedElement(
        *elements, nullptr, CSF_STR(0x00841304, ".PANEL_PRINCIPAL"));

    void* panel =
        reinterpret_cast<void* (__fastcall*)(void*, void*, uint8_t*)>(
            factoryVtable[0x18 / 4])(factory, nullptr, panelElement);
    CSF_AT(void*, self, 0x13C) = panel;

    if (panel)
    {
        void* display = LookupObjectById(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        void* dimensions =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);
        const unsigned int width = CSF_AT(unsigned int, dimensions, 0);

        display = LookupObjectById(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        dimensions =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);
        const unsigned int height = CSF_AT(unsigned int, dimensions, 4);

        panel = CSF_AT(void*, self, 0x13C);
        void** panelVtable = CSF_AT(void**, panel, 0);
        // Unsigned conversion naturally replaces the x87 bias adjustment.
        const float heightScale =
            static_cast<float>(height) / static_cast<float>(g_ScreenHeight);
        const float widthScale =
            static_cast<float>(width) / static_cast<float>(g_ScreenWidth);

        reinterpret_cast<void (__fastcall*)(void*, void*, float, float)>(
            panelVtable[0x150 / 4])(panel, nullptr, widthScale, heightScale);

        panel = CSF_AT(void*, self, 0x13C);
        void* continueButton =
            reinterpret_cast<void* (__fastcall*)(void*, void*, const char*)>(
                CSF_AT(void**, panel, 0)[0xFC / 4])(
                    panel, nullptr, CSF_STR(0x00841F78, "Continue"));
        CSF_AT(void*, self, 0x138) = continueButton;

        if (continueButton)
        {
            reinterpret_cast<void (__fastcall*)(void*, void*)>(
                CSF_AT(void**, self, 0)[0x90 / 4])(self, nullptr);
        }
    }

    // The packet also identifies the initializer address as a vtable reference.
    (void)CSF_VTBL(0x00542700);
    void* entry = Mem_Alloc(0x10);
    CSF_AT(void*, self, 0x14C) = entry;
    return MenuMPCharSel_InitEntry(entry, nullptr, reinterpret_cast<int>(self));
}

HOOK(0x00535C90, Interface_InitializeMainPanel, "auto");
