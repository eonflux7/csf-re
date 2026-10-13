#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360, (uint8_t* self, void* edx, const char* name));
CSF_FUNC(void, __cdecl, CoreVariantValues_CopyWideStringBounded, 0x006956C0, (int destBase, uint16_t* srcStr, int maxCount));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(unsigned int, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(unsigned int, g_ScreenHeight, 0x00899BA0);

static int __fastcall CMenuMpStats_InitializePanel(
    CMenuMpStats* self, void* /*edx*/, int managerAddress, void** elements)
{
    if (self->field_0084)
        return static_cast<int>(self->field_0084);

    void* manager = reinterpret_cast<void*>(managerAddress);
    void* panelFactory =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, manager, 0)[0x54 / 4])(manager, nullptr);

    void** factoryVtable = CSF_AT(void**, panelFactory, 0);
    uint8_t* panelElement = LookupNamedElement(
        reinterpret_cast<uint8_t*>(*elements), nullptr,
        CSF_STR(0x00841304, ".PANEL_PRINCIPAL"));

    void* panel =
        reinterpret_cast<void* (__fastcall*)(void*, void*, uint8_t*)>(
            factoryVtable[0x18 / 4])(panelFactory, nullptr, panelElement);
    self->field_0084 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(panel));
    if (!panel)
        return 0;

    void* display = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* dimensions =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);
    unsigned int width = CSF_AT(unsigned int, dimensions, 0);

    display = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    dimensions =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);
    unsigned int height = CSF_AT(unsigned int, dimensions, 0x4);

    panel = reinterpret_cast<void*>(self->field_0084);
    float heightScale = static_cast<float>(height) / static_cast<float>(g_ScreenHeight);
    float widthScale = static_cast<float>(width) / static_cast<float>(g_ScreenWidth);
    reinterpret_cast<void (__fastcall*)(void*, void*, float, float)>(
        CSF_AT(void**, panel, 0)[0x150 / 4])(
            panel, nullptr, widthScale, heightScale);

    panel = reinterpret_cast<void*>(self->field_0084);
    void* team1 =
        reinterpret_cast<void* (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, panel, 0)[0xFC / 4])(
                panel, nullptr, CSF_STR(0x008431D4, "TextoTeam1"));

    panel = reinterpret_cast<void*>(self->field_0084);
    self->field_0014 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(team1));
    void* team2 =
        reinterpret_cast<void* (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, panel, 0)[0xFC / 4])(
                panel, nullptr, CSF_STR(0x008431C8, "TextoTeam2"));

    team1 = reinterpret_cast<void*>(self->field_0014);
    self->field_0018 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(team2));
    if (!team1 || !team2)
        return static_cast<int>(reinterpret_cast<uintptr_t>(team2));

    void* textValue =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, team1, 0)[0x110 / 4])(team1, nullptr);
    uint16_t* text =
        reinterpret_cast<uint16_t* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, textValue, 0)[0x44 / 4])(textValue, nullptr);
    CoreVariantValues_CopyWideStringBounded(
        static_cast<int>(reinterpret_cast<uintptr_t>(&self->field_001c)), text, 25);

    team2 = reinterpret_cast<void*>(self->field_0018);
    textValue =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, team2, 0)[0x110 / 4])(team2, nullptr);
    text =
        reinterpret_cast<uint16_t* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, textValue, 0)[0x44 / 4])(textValue, nullptr);
    CoreVariantValues_CopyWideStringBounded(
        static_cast<int>(reinterpret_cast<uintptr_t>(&self->field_004e)), text, 25);

    // The original leaves the void copy routine's unspecified EAX here.
    return 0;
}

HOOK(0x5417C0, CMenuMpStats_InitializePanel, "auto");
