#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CoreParser_CopyAttributeText, 0x00692D40, (uint8_t* self, void* , char* buffer, int capacity));
CSF_FUNC(S_00692b80*, __fastcall, LookupNamedElement, 0x00693360, (S_00692b80* self, void* , char* String1));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(uint32_t, g_DefaultTextDatabasePath, 0x0083EE8C);
CSF_GLOBAL(uint32_t, g_DefaultTextDatabasePathWord1, 0x0083EE90);
CSF_GLOBAL(uint32_t, g_DefaultTextDatabasePathWord2, 0x0083EE94);
CSF_GLOBAL(uint32_t, g_DefaultTextDatabasePathWord3, 0x0083EE98);
CSF_GLOBAL(uint32_t, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(uint32_t, g_ScreenHeight, 0x00899BA0);

static int __fastcall CMenuEula_InitializeMainPanel(
    CMenuEula* self, void* /*edx*/, void* provider, S_00692b80** document)
{
    if (self->field_001c)
        return static_cast<int>(self->field_001c);

    void* panelFactory =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, provider, 0)[0x54 / 4])(provider, nullptr);

    void** factoryVtable = CSF_AT(void**, panelFactory, 0);
    S_00692b80* panelElement = LookupNamedElement(
        *document, nullptr,
        const_cast<char*>(CSF_STR(0x00841304, ".PANEL_PRINCIPAL")));

    void* panel =
        reinterpret_cast<void* (__fastcall*)(void*, void*, S_00692b80*)>(
            factoryVtable[0x18 / 4])(panelFactory, nullptr, panelElement);

    self->field_001c = reinterpret_cast<uint32_t>(panel);
    if (!panel)
        return 0;

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
    uint32_t height = CSF_AT(uint32_t, dimensions, 4);

    panel = reinterpret_cast<void*>(self->field_001c);
    void* resizeMethod = CSF_AT(void**, panel, 0)[0x150 / 4];
    float normalizedHeight =
        static_cast<float>(height) / static_cast<float>(g_ScreenHeight);
    float normalizedWidth =
        static_cast<float>(width) / static_cast<float>(g_ScreenWidth);

    reinterpret_cast<void (__fastcall*)(void*, void*, float, float)>(
        resizeMethod)(panel, nullptr, normalizedWidth, normalizedHeight);

    S_00692b80* fileElement = LookupNamedElement(
        *document, nullptr,
        const_cast<char*>(CSF_STR(0x00841B64, ".FICHERO_EIDOS")));

    if (fileElement)
    {
        return CoreParser_CopyAttributeText(
            reinterpret_cast<uint8_t*>(fileElement), nullptr,
            &CSF_AT(char, self, 0x58), 256);
    }

    CSF_AT(uint32_t, self, 0x58) = g_DefaultTextDatabasePath;
    uint32_t result = g_DefaultTextDatabasePathWord1;
    CSF_AT(uint32_t, self, 0x5C) = result;
    CSF_AT(uint32_t, self, 0x60) = g_DefaultTextDatabasePathWord2;
    CSF_AT(uint32_t, self, 0x64) = g_DefaultTextDatabasePathWord3;
    return static_cast<int>(result);
}

HOOK(0x00532A40, CMenuEula_InitializeMainPanel, "auto");
