#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int*, __cdecl, CMenuLanguage_ClearPageState, 0x0050C300, (int* a1));
CSF_FUNC(int, __fastcall, CMenuLanguage_InitializeControls, 0x005345D0, (void* self, void* edx, int* a3));
CSF_FUNC(int, __cdecl, CMenuLanguage_CopyPageState, 0x005348B0, (void* a1, void* arg4));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(S_00692b80*, __fastcall, LookupNamedElement, 0x00693360, (S_00692b80* self, void* , char* String1));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(unsigned int, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(unsigned int, g_ScreenHeight, 0x00899BA0);

static int __fastcall CMenuLanguage_InitializeMainPanel(
    CMenuLanguage* self, void* /*edx*/,
    int* controller, S_00692b80** elements)
{
    if (self->field_0030)
        return static_cast<int>(self->field_0030);

    auto* pageState = reinterpret_cast<uint8_t*>(self) + 0x10;
    CMenuLanguage_ClearPageState(reinterpret_cast<int*>(pageState));

    auto* globalState =
        reinterpret_cast<uint8_t*>(GetGlobalValue_75FDF0());
    CMenuLanguage_CopyPageState(globalState + 0x1C, pageState);

    void* panelCollection =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, controller, 0)[0x54 / 4])(
                controller, nullptr);
    void** collectionVtable = CSF_AT(void**, panelCollection, 0);

    S_00692b80* namedPanel = LookupNamedElement(
        *elements, nullptr,
        const_cast<char*>(CSF_STR(0x00841304, ".PANEL_PRINCIPAL")));

    int result = reinterpret_cast<int (__fastcall*)(void*, void*, S_00692b80*)>(
        collectionVtable[0x18 / 4])(panelCollection, nullptr, namedPanel);
    self->field_0030 = static_cast<uint32_t>(result);

    if (!result)
        return result;

    CMenuLanguage_InitializeControls(self, nullptr, controller);

    void* display = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* dimensions =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);
    uint32_t width = CSF_AT(uint32_t, dimensions, 0);

    display = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    dimensions =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);
    uint32_t height = CSF_AT(uint32_t, dimensions, 0x4);

    void* panel = reinterpret_cast<void*>(self->field_0030);
    void** panelVtable = CSF_AT(void**, panel, 0);

    // Native unsigned conversions replace the original x87 bias adjustment.
    float heightScale = static_cast<float>(height) /
                        static_cast<float>(g_ScreenHeight);
    float widthScale = static_cast<float>(width) /
                       static_cast<float>(g_ScreenWidth);

    return reinterpret_cast<int (__fastcall*)(void*, void*, float, float)>(
        panelVtable[0x150 / 4])(panel, nullptr, widthScale, heightScale);
}

HOOK(0x005349E0, CMenuLanguage_InitializeMainPanel, "auto");
